/*
 * bluetooth.h
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "hci.h"

enum BluetoothStates
{
    BTS_start,
    BTS_test,
    BTS_end
};

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();
    void handler(void);
private:
    BluetoothStates state;
    HCI* pHci;
};

#endif /* BLUETOOTH_H_ */
