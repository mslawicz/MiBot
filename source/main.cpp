#include "Logger.h"
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

    BLE& mydevicename = BLE::Instance();

    while (true)
    {

    }
}

