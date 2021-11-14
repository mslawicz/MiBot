#include "Logger.h"
#include <BleProcess.h>
#include "ble/FunctionPointerWithContext.h"
//#include <GattServer.h>
#include <algorithm>
#include <iomanip>

/**
    * Construct a BLEProcess from an event queue and a ble interface.
    *
    * Call start() to initiate ble processing.
    */
BleProcess::BleProcess(events::EventQueue& event_queue) :
    _event_queue(event_queue),
    _ble_interface(BLE::Instance())
{
}

/**
    * Subscription to the ble interface initialization event.
    *
    * @param[in] cb The callback object that will be called when the ble
    * interface is initialized.
    */
void BleProcess::setOnInitCbk(mbed::Callback<void()> cb)
{
    _post_init_cb = cb;
}

/**
* Initialize the ble interface, configure it and start advertising.
*
* @return true on success
*/
bool BleProcess::start()
{
    LOG_DEBUG("BLE process is starting");

    if (_ble_interface.hasInitialized())
    {
        LOG_ERROR("the BLE instance has already been initialized");
        return false;
    }

    //assign BLE events process callback
    _ble_interface.onEventsToProcess(makeFunctionPointer(this, &BleProcess::scheduleBleEvents));

    //initialize BLE interface with on_complete callback
    ble_error_t error = _ble_interface.init(this, &BleProcess::whenInitComplete);

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " returned by BLE::init");
        return false;
    }

    return true;
}

/**
* Close existing connections and stop the process.
*/
void BleProcess::stop()
{
    if (_ble_interface.hasInitialized())
    {
        _ble_interface.shutdown();
        LOG_DEBUG("BLE process stopped");
    }
}

/**
* Schedule processing of events from the BLE middleware in the event queue.
*/
void BleProcess::scheduleBleEvents(BLE::OnEventsToProcessCallbackContext* context)
{
    _event_queue.call(mbed::callback(&context->ble, &BLE::processEvents));
}

/**
* Sets up adverting payload and start advertising.
*
* This function is invoked when the ble interface is initialized.
*/
void BleProcess::whenInitComplete(BLE::InitializationCompleteCallbackContext* event)
{
    if (event->error != BLE_ERROR_NONE)
    {
        LOG_ERROR(event->error << " during the BLE initialization");
        return;
    }
    
    LOG_INFO("BLE instance initialized");

    //take reference to gap object
    Gap& gap = _ble_interface.gap();

    gap.setEventHandler(this);

    if (!setAdvertisingParameters())
    {
        return;
    }

    if (!setAdvertisingData())
    {
        return;
    }

    printMacAddress();

    if (!startAdvertising())
    {
        return;
    }

    if (_post_init_cb)
    {
        _post_init_cb();
    }
    else
    {
        LOG_WARNING("BLE post init callback has not been set");
    }
   
}

bool BleProcess::setAdvertisingParameters()
{
    Gap &gap = _ble_interface.gap();

    static const ble::AdvertisingParameters advertisingParams
    (
        ble::advertising_type_t::CONNECTABLE_UNDIRECTED,
        ble::adv_interval_t(ble::millisecond_t(100)),       //minimum interval in ms
        ble::adv_interval_t(ble::millisecond_t(200))        //maximum interval in ms
    );

    ble_error_t error = gap.setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, advertisingParams);

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR("Gap::setAdvertisingParameters() failed with error " << error);
        return false;
    }

    LOG_INFO("Gap::setAdvertisingParameters() succeeded");
    return true;
}

bool BleProcess::setAdvertisingData()
{
    Gap &gap = _ble_interface.gap();

    //XXX UUID services[]{GattService::UUID_BATTERY_SERVICE};  //NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)

    //Use the simple builder to construct the payload
    _dataBuilder
    .setFlags()
    .setName("MiBot");
    //XXX.setLocalServiceList({static_cast<UUID*>(services), sizeof(services) / sizeof(services[0])})
    //XXX.setAppearance(ble::adv_data_appearance_t::);

    ble_error_t error = gap.setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE, _dataBuilder.getAdvertisingData());

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR("Gap::setAdvertisingPayload() failed with error " << error);
        return false;
    }

    LOG_INFO("Gap::setAdvertisingPayload() succeeded");
    return true;
}

/**
* Start adverting
*
* This function is invoked when the ble interface is initialized and post init callback is set
*/
bool BleProcess::startAdvertising()
{
    Gap& gap = _ble_interface.gap();

    /* Start advertising the set */
    ble_error_t error = gap.startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " during gap.startAdvertising");
        return false;
    }

    LOG_INFO("Advertising start command");
    return true;
}

/**
* Called when connection attempt ends. Check event.getStatus() to see if connection
* was established. If this device is the peripheral and it was advertising this will
* end the advertising set which will also create the onAdvertisingEnd event.
*/
void BleProcess::onConnectionComplete(const ble::ConnectionCompleteEvent&  event)
{
    LOG_INFO("BLE client connected with status " << event.getStatus());
}

/*
Called when a connection has been disconnected
*/
void BleProcess::onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event)
{
    LOG_INFO("BLE client disconnected for reason " << static_cast<int>(event.getReason().value()));
    startAdvertising();
}

void BleProcess::printMacAddress()
{
    ble::own_address_type_t addr_type;
    ble::address_t addr;
    auto error = _ble_interface.gap().getAddress(addr_type, addr);
    if(error != BLE_ERROR_NONE)
    {
        LOG_ERROR("cannot get MAC address, error " << error);
        return;
    }
 
    LOG_DEBUG("device MAC address: " << std::hex
        << static_cast<int>(addr[5]) << " "
        << static_cast<int>(addr[4]) << " "
        << static_cast<int>(addr[3]) << " "
        << static_cast<int>(addr[2]) << " "
        << static_cast<int>(addr[1]) << " "
        << static_cast<int>(addr[0]));
}

/**
* Called when advertising starts.
*
* @param event Advertising start event.
*
* @note Check event.getStatus() to see if advertising started successfully
*/
void BleProcess::onAdvertisingStart(const ble::AdvertisingStartEvent& event)
{
    LOG_INFO("BLE advertising (set " << event.getStatus() << ") started with status " << event.getStatus());
}

/**
* Called when advertising ends.
*
* Advertising ends when the process timeout or if it is stopped by the
* application or if the local device accepts a connection request.
*
* @param event Advertising end event.
*
* @note Check event.getStatus() to see if advertising ended successfully
*/
void BleProcess::onAdvertisingEnd(const ble::AdvertisingEndEvent& event)
{
    LOG_INFO("BLE advertising ends for reason " << static_cast<int>(event.getCompleted_events()) << " with status " << event.getStatus());
}