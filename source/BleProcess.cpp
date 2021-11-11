#include "Logger.h"
#include <BleProcess.h>
//#include "ble/GapAdvertisingParams.h"
//#include "ble/GapAdvertisingData.h"
#include "ble/FunctionPointerWithContext.h"

/**
    * Construct a BLEProcess from an event queue and a ble interface.
    *
    * Call start() to initiate ble processing.
    */
BleProcess::BleProcess(events::EventQueue &event_queue, BLE &ble_interface) :
    _event_queue(event_queue),
    _ble_interface(ble_interface)
{
}

/**
    * Subscription to the ble interface initialization event.
    *
    * @param[in] cb The callback object that will be called when the ble
    * interface is initialized.
    */
void BleProcess::setOnInitCbk(mbed::Callback<void(BLE&, events::EventQueue&)> cb)
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

    _ble_interface.onEventsToProcess(makeFunctionPointer(this, &BleProcess::scheduleBleEvents));

    ble_error_t error = _ble_interface.init(this, &BleProcess::whenInitComplete);

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " returned by BLE::init");
        return false;
    }

    _ble_interface.securityManager().init();
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
void BleProcess::scheduleBleEvents(BLE::OnEventsToProcessCallbackContext* event)
{
    _event_queue.call(mbed::callback(&event->ble, &BLE::processEvents));
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

    if (!startAdvertising())
    {
        return;
    }    

    if (_post_init_cb)
    {
        _post_init_cb(_ble_interface, _event_queue);
    }
    else
    {
        LOG_WARNING("BLE post init callback has not been set");
    }
   
}

bool BleProcess::setAdvertisingParameters()
{
    Gap &gap = _ble_interface.gap();

    ble_error_t error = gap.setAdvertisingParameters(
        ble::LEGACY_ADVERTISING_HANDLE,
        ble::AdvertisingParameters()
    );

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

    /* Use the simple builder to construct the payload; it fails at runtime
    * if there is not enough space left in the buffer */
    ble_error_t error = gap.setAdvertisingPayload
    (
        ble::LEGACY_ADVERTISING_HANDLE,
        ble::AdvertisingDataSimpleBuilder<ble::LEGACY_ADVERTISING_MAX_SIZE>()
            .setFlags()
            .setName("MiBot")
            .getAdvertisingData()
    );

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

    LOG_INFO("Advertising started");
    return true;
}

/*
Called when connection attempt ends or an advertising device has been connected
*/
void BleProcess::onConnectionComplete(const ble::ConnectionCompleteEvent& event)
{
    LOG_INFO("BLE client connected");
}

/*
Called when a connection has been disconnected
*/
void BleProcess::onDisconnectionComplete(const ble::DisconnectionCompleteEvent&  /*event*/)
{
    LOG_INFO("BLE client disconnected");
    startAdvertising();
}