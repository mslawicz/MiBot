#include "Logger.h"
#include <BleProcess.h>
#include "ble/FunctionPointerWithContext.h"
//#include <GattServer.h>
#include <algorithm>
#include <iomanip>
#include <sstream>

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

    ble::own_address_type_t addr_type;
    ble::address_t addr;
    ble_error_t error = _ble_interface.gap().getAddress(addr_type, addr);
    if(error != BLE_ERROR_NONE)
    {
        LOG_ERROR("cannot get MAC address, error " << error);
        return;
    }
 
    printMacAddress(addr.data());

    /* This path will be used to store bonding information but will fallback
        * to storing in memory if file access fails (for example due to lack of a filesystem) */
    const char* db_path = "/fs/bt_sec_db";

    error = _ble_interface.securityManager().init
    (
        /* enableBonding */ true,
        /* requireMITM */ false,
        /* iocaps */ SecurityManager::IO_CAPS_NONE,
        /* passkey */ nullptr,
        /* signing */ false,
        /* dbFilepath */ db_path
    );

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " during initialising security manager");
        return;
    }

    /* This tells the stack to generate a pairingRequest event which will require
    * this application to respond before pairing can proceed. Setting it to false
    * will automatically accept pairing. */
    _ble_interface.securityManager().setPairingRequestAuthorisation(true);

#if MBED_CONF_APP_FILESYSTEM_SUPPORT
    error = _ble_interface.securityManager().preserveBondingStateOnReset(true);

    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << " during preserveBondingStateOnReset");
    }
#endif // MBED_CONF_APP_FILESYSTEM_SUPPORT

    /* Tell the security manager to use methods in this class to inform us
    * of any events. Class needs to implement SecurityManagerEventHandler. */
    _ble_interface.securityManager().setSecurityManagerEventHandler(this);

    //take reference to gap object
    Gap& gap = _ble_interface.gap();

    gap.setEventHandler(this);

    error = gap.enablePrivacy(true);
    if (error != BLE_ERROR_NONE)
    {
        LOG_ERROR(error << "Error enabling privacy");
        return;
    }  

    /* continuation is in onPrivacyEnabled() */   
}

void BleProcess::configureAdvertising()
{
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

    //Use the simple builder to construct the payload
    _dataBuilder
    .setFlags()
    .setName("MiBot");

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

void BleProcess::printMacAddress(const uint8_t* pAddr)
{
    stringstream ss;
    constexpr uint8_t NoOfBytes = 6;
    ss << std::hex << std::setfill('0') << std::setw(2);
    for(uint8_t byte=0; byte<NoOfBytes; byte++)
    {
        ss << static_cast<int>(pAddr[0]);    //NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if(byte < NoOfBytes - 1)
        {
            ss << ":";
        }
    }

    LOG_DEBUG("device MAC address: " << ss.str());
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

/** Respond to a pairing request. This will be called by the stack
    * when a pairing request arrives and expects the application to
    * call acceptPairingRequest or cancelPairingRequest */
void BleProcess::pairingRequest(ble::connection_handle_t connectionHandle)
{
    LOG_INFO("Pairing requested - authorising");
    _ble_interface.securityManager().acceptPairingRequest(connectionHandle);
}

/** Inform the application of pairing */
void BleProcess::pairingResult(ble::connection_handle_t connectionHandle, SecurityManager::SecurityCompletionStatus_t result)
{
    if (result == SecurityManager::SEC_STATUS_SUCCESS)
    {
        LOG_INFO("Pairing successful");
        _bonded = true;
    } 
    else
    {
        LOG_ERROR("Pairing failed");
    }

    _event_queue.call_in(_delay, [this, connectionHandle] { _ble_interface.gap().disconnect(connectionHandle, ble::local_disconnection_reason_t::USER_TERMINATION); });
}

/** Inform the application of change in encryption status. This will be
    * communicated through the serial port */
void BleProcess::linkEncryptionResult(ble::connection_handle_t  /*connectionHandle*/, ble::link_encryption_t result)
{
    if (result == ble::link_encryption_t::ENCRYPTED)
    {
        LOG_INFO("Link ENCRYPTED");
    }
    else if (result == ble::link_encryption_t::ENCRYPTED_WITH_MITM)
    {
        LOG_INFO("Link ENCRYPTED_WITH_MITM");
    }
    else if (result == ble::link_encryption_t::NOT_ENCRYPTED)
    {
        LOG_INFO("Link NOT_ENCRYPTED");
    }
}

void BleProcess::onPrivacyEnabled()
{
    /* all initialisation complete, start our main activity */
    LOG_INFO("Privacy enabled");
    configureAdvertising();
}