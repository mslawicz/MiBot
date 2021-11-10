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
};

#endif /* BLE_PROCESS_H_ */