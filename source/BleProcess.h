#ifndef BLE_PROCESS_H_
#define BLE_PROCESS_H_

#include "ble/BLE.h"
#include <mbed.h>

/**
 * Handle initialization and shutdown of the BLE Instance.
 *
 * Setup advertising payload and manage advertising state.
 * Delegate to GattClientProcess once the connection is established.
 */
class BleProcess : private mbed::NonCopyable<BleProcess>, public ble::Gap::EventHandler
{
public:
    BleProcess(events::EventQueue &event_queue, BLE &ble_interface);
    ~BleProcess() { stop(); }
    bool start();
    void stop();
    void setOnInitCbk(mbed::Callback<void(BLE&, events::EventQueue&)> cb);
private:
    void scheduleBleEvents(BLE::OnEventsToProcessCallbackContext* event);
    void whenInitComplete(BLE::InitializationCompleteCallbackContext* event);
    bool setAdvertisingParameters();
    bool setAdvertisingData();
    bool startAdvertising();
    events::EventQueue& _event_queue;
    BLE& _ble_interface;
    mbed::Callback<void(BLE&, events::EventQueue&)> _post_init_cb{nullptr};
};

#endif /* BLE_PROCESS_H_ */