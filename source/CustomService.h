 
#ifndef CUSTOM_SERVICE_H_
#define CUSTOM_SERVICE_H_

#include "Logger.h"
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/services/BatteryService.h"

class CustomService : ble::GattServer::EventHandler
{
public:
    CustomService(events::EventQueue& eventQueue, BLE& bleInterface);
    void start();
private:
    void onDataWritten(const GattWriteCallbackParams &params) override;
    void onDataSent(const GattDataSentCallbackParams & /*params*/) override { LOG_DEBUG("!!! onDataSent"); }
    void onDataRead(const GattReadCallbackParams & /*params*/) override { LOG_DEBUG("!!! onDataRead"); }
    void onUpdatesEnabled(const GattUpdatesEnabledCallbackParams & /*params*/) override { LOG_DEBUG("!!! onUpdatesEnabled"); }
    void onUpdatesDisabled(const GattUpdatesDisabledCallbackParams & /*params*/) override { LOG_DEBUG("!!! onUpdatesDisabled"); }
    void onConfirmationReceived(const GattConfirmationReceivedCallbackParams & /*params*/) override { LOG_DEBUG("!!! onConfirmationReceived"); }
    events::EventQueue& _eventQueue;     // event queue of the main thread
    BLE& _bleInterface;                  // interface to BLE device
    static constexpr size_t UserDataSize = 4;
    WriteOnlyArrayGattCharacteristic<uint8_t, UserDataSize>* _pWritableCharacteristic{nullptr};
    uint8_t _characteristicValue[UserDataSize]{0};    //NOLINT(hicpp-avoid-c-arrays,modernize-avoid-c-arrays,cppcoreguidelines-avoid-c-arrays)
    const char* CustomServiceUUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"; // Nordic UART Service 
    const char* WritableCharacteristicUUID = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";    // write characteristic
    uint8_t _batteryLevel{0};
    BatteryService* _pBatteryService{nullptr};
};

#endif /* CUSTOM_SERVICE_H_ */