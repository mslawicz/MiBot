/*
 * bluetooth.cpp
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#include "bluetooth.h"
#include "console.h"//XXX
#include "system.h"//XXX
#include <string>//XXX

Bluetooth::Bluetooth()
{
    state = BluetoothStates::BTS_start;
    pHci = new HCI(SpiBus::pSpi1, BLUETOOTH_CS_PORT, BLUETOOTH_CS_PIN);
}

Bluetooth::~Bluetooth()
{
    delete pHci;
}

/*
 * Bluetooth handler
 * state machine to be executed periodically in a loop
 */
void Bluetooth::handler(void)
{
    pHci->handler();
    if(!pHci->getEventQueue().empty()) //XXX
    {
        auto event = pHci->getEventQueue().front();
        pHci->getEventQueue().pop();
        std::string eventData("event: ");
        for(auto byte : event)
        {
            eventData += Console::toHex(byte, 2, false);
            eventData += ",";
        }
        System::getInstance().getConsole()->sendMessage(Severity::Info, eventData);
    }
}
