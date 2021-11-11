#include "Logger.h"
#include "BleProcess.h"
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
    LOG_INFO("MiBot v1.0");

    // main event queue
    events::EventQueue eventQueue;

    //BLE interface instance
    BLE& bleInterface = BLE::Instance();

    //construct BLE process
    BleProcess bleProcess(eventQueue, bleInterface);

    //assign BLE on init callback function
    bleProcess.on_init(callback([](BLE&  /*bleInterface*/, events::EventQueue&  /*eventQueue*/){ LOG_INFO("executing on init callback");}));

    // bind the event queue to the ble interface, initialize the interface and start advertising
    bleProcess.start();

    // process the event queue
    eventQueue.dispatch_forever();

    // this should never be reached
    return 0;
}

