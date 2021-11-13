#ifndef BLE_PROCESS_H_
#define BLE_PROCESS_H_

#include "ble/BLE.h"
#include "ble/Gap.h"
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
    explicit BleProcess(events::EventQueue& event_queue);
    ~BleProcess() { stop(); }
    BleProcess(const BleProcess&) = delete;
    BleProcess(const BleProcess&&) = delete;
    BleProcess& operator=(const BleProcess&) = delete;
    BleProcess& operator=(const BleProcess&&) = delete;
    bool start();
    void stop();
    void setOnInitCbk(mbed::Callback<void()> cb);
private:
    void scheduleBleEvents(BLE::OnEventsToProcessCallbackContext* event);
    void whenInitComplete(BLE::InitializationCompleteCallbackContext* event);
    bool setAdvertisingParameters();
    bool setAdvertisingData();
    bool startAdvertising();
    void onConnectionComplete(const ble::ConnectionCompleteEvent& event) override;
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent& event) override;
    void printMacAddress();
    void onAdvertisingStart(const ble::AdvertisingStartEvent& event) override;
    void onAdvertisingEnd(const ble::AdvertisingEndEvent& event) override;
    events::EventQueue& _event_queue;
    BLE& _ble_interface;
    mbed::Callback<void()> _post_init_cb{nullptr};
    ble::AdvertisingDataSimpleBuilder<ble::LEGACY_ADVERTISING_MAX_SIZE> _dataBuilder;
};

#endif /* BLE_PROCESS_H_ */
