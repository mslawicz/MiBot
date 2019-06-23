/*
 * bluetooth.cpp
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#include "bluetooth.h"

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
    switch(state)
    {
    case BTS_start:
        state = BTS_test;
        break;
    case BTS_test:
        pHci->sendCommand(3, 3, std::vector<uint8_t>{0});
        state = BTS_end;
        break;
    case BTS_end:
        break;
    default:
        break;
    }
}
