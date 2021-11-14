#include "CustomService.h"
#include <cstdlib>

CustomService::CustomService(events::EventQueue& eventQueue, BLE& bleInterface) :
    _eventQueue(eventQueue),
    _bleInterface(bleInterface)
{
    _writable_characteristic = new ReadWriteGattCharacteristic<uint8_t>(WritableCharacteristicUUID, &_characteristicValue);   //NOLINT (cppcoreguidelines-owning-memory)
    if(_writable_characteristic == nullptr)
    {
        LOG_ERROR("Allocation of ReadWriteGattCharacteristic failed");
    }
}    

void CustomService::start()
{
    LOG_INFO("GATT custom service start");
    //create array of all used characteristics
    GattCharacteristic* characteristics[] = { _writable_characteristic }; //NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    auto numberOfCharacteristics = sizeof(characteristics) / sizeof(characteristics[0]);
    //create service with the list of characteristics
    GattService dataService(CustomServiceUUID, static_cast<GattCharacteristic**>(characteristics), numberOfCharacteristics);

    //register service in GATT server
    ble_error_t error = _bleInterface.gattServer().addService(dataService);
    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " during GATT service registration");
        return;
    }

    for(size_t index = 0; index < numberOfCharacteristics; index++)
    {
        LOG_DEBUG("GATT characteristic #" << index << " registered with handle " << characteristics[index]->getValueHandle());  //NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        // set security mode
        //characteristics[index]->setWriteSecurityRequirement(GattCharacteristic::SecurityRequirement_t::SC_AUTHENTICATED);   //NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    //add battery service to GATT server
    _pBatteryService = new BatteryService(_bleInterface);   //NOLINTerror = _bleInterface.gattServer().addService(dataService);

    //XXX battery level test
    _eventQueue.call_every(1s, [this]()
    {
        _batteryLevel = 50U +rand() % 10U;
        _pBatteryService->updateBatteryLevel(_batteryLevel);
    });

    //dispatch GATT events in this object
    _bleInterface.gattServer().setEventHandler(this);
}

/**
* Handler called after an attribute has been written.
*/
void CustomService::onDataWritten(const GattWriteCallbackParams& params)
{
    if(params.handle == _writable_characteristic->getValueHandle())
    {
        LOG_DEBUG("Received data length " << params.len << " with the first byte " << static_cast<int>(*(params.data)));
    }
    else
    {
        LOG_ERROR("Characteristic write attempt with unknown handle " << params.handle);
    }
}
