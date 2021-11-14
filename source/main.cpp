#include "Logger.h"
#include "BleProcess.h"
#include "CustomService.h"
#include "ble/BLE.h"
#include "mbed.h"
#include <iostream>

// main() runs in its own thread in the OS
int main()
{
#ifdef MBED_DEBUG
    HAL_DBGMCU_EnableDBGSleepMode();
#endif

    logTimer.start();
    LOG_ALWAYS("MiBot v1.0");

    // main event queue
    events::EventQueue eventQueue;

    //construct BLE process
    BleProcess bleProcess(eventQueue);

    //construct GATT custom service object
    CustomService customService(eventQueue, bleProcess.getBleInterface());

    //assign BLE on init callback function
    bleProcess.setOnInitCbk(callback(&customService, &CustomService::start));

    // bind the event queue to the ble interface, initialize the interface and start advertising
    bleProcess.start();

    // process the event queue
    eventQueue.dispatch_forever();

    // this should never be reached
    return 0;
}
