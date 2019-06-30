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
#include "timer.h"//XXX

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
    static Timer timer;
    pHci->handler();
    if(!pHci->getEventQueue().empty())
    {
        // there's an event in the queue
        auto event = pHci->getEventQueue().front();
        pHci->getEventQueue().pop();
        eventHandler(event);
    }
//    if(timer.elapsed(500000))
//    {
//        timer.reset();
//        //XXX send: HCI_READ_LOCAL_VERSION_INFORMATION
//        pHci->sendCommand(0x1001);
//    }
}

/*
 * HCI event handler
 */
void Bluetooth::eventHandler(std::vector<uint8_t> event)
{
    std::string eventData("HCI event: ");
    for(auto byte : event)
    {
        eventData += Console::toHex(byte, 2, false);
        eventData += ",";
    }
    System::getInstance().getConsole()->sendMessage(Severity::Debug, LogChannel::LC_BLUETOOTH, eventData);

    // check the event code
    switch(event[1])
    {
    case 0xFF:  // vendor specific event
        vsEventHandler(event);
        break;
    default:
        System::getInstance().getConsole()->sendMessage(Severity::Error, LogChannel::LC_BLUETOOTH, "unknown HCI event code: " + Console::toHex(event[1], 2));
        break;
    }
}

/*
 * HCI vendor specific event handler
 */
void Bluetooth::vsEventHandler(std::vector<uint8_t> event)
{
    uint16_t eventCode = *reinterpret_cast<uint16_t*>(&event[3]);
    switch(eventCode)
    {
    case 0x0001:    // Evt_Blue_Initialized
        switch(event[5])
        {
        case 0x01:
            System::getInstance().getConsole()->sendMessage(Severity::Info, LogChannel::LC_BLUETOOTH, "BlueNRG-MS firmware started properly");
            break;
        default:
            System::getInstance().getConsole()->sendMessage(Severity::Error, LogChannel::LC_BLUETOOTH, "BlueNRG-MS firmware started with code: " + Console::toHex(event[5], 2));
            break;
        }
        break;
    default:
        System::getInstance().getConsole()->sendMessage(Severity::Error, LogChannel::LC_BLUETOOTH, "unknown HCI vendor specific event code: " + Console::toHex(eventCode, 4));
        break;
    }
}
