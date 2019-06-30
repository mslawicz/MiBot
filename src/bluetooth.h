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
    BTS_start
};

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();
    void handler(void);
private:
    void eventHandler(std::vector<uint8_t> event);
    void vsEventHandler(std::vector<uint8_t> event);
    BluetoothStates state;
    HCI* pHci;
};

#endif /* BLUETOOTH_H_ */
