#include "Logger.h"
#include "CustomService.h"
#include "BleProcess.h"
#include "ble/BLE.h"
#include "mbed.h"
#include <iostream>

CustomService* pCustomService = nullptr;

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

    //BLE interface instance
    BLE& bleInterface = BLE::Instance();

    //construct BLE process
    BleProcess bleProcess(eventQueue, bleInterface);

    //create GATT server (this can be moved to main control function when available)
    CustomService movementControl(eventQueue, bleInterface);
    pCustomService = &movementControl;

    //assign BLE on init callback function
    bleProcess.setOnInitCbk(callback([](BLE&   /*bleInterface*/, events::EventQueue&   /*eventQueue*/)
    {
        LOG_INFO("executing on init callback");
        pCustomService->start();
    }));

    // bind the event queue to the ble interface, initialize the interface and start advertising
    bleProcess.start();

    // process the event queue
    eventQueue.dispatch_forever();

    // this should never be reached
    return 0;
}
