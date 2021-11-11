#include <BleProcess.h>

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
void BleProcess::on_init(mbed::Callback<void(BLE&, events::EventQueue&)> cb)
{
    _post_init_cb = cb;
}