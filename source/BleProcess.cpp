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