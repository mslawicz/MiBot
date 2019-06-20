/*
 * bluetooth.cpp
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#include "bluetooth.h"
#include "hci.h"

Bluetooth::Bluetooth()
{
    state = BluetoothStates::BTS_start;
}

Bluetooth::~Bluetooth()
{
    // TODO Auto-generated destructor stub
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
        state = BTS_hciInit;
        break;
    case BTS_hciInit:
        hci_init(userNotify, nullptr);
        state = BTS_afterHciInit;
        break;
    default:
        break;
    }
}
