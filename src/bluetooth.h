/*
 * bluetooth.h
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_


enum BluetoothStates
{
    BTS_start,
    BTS_hciInit,
    BTS_afterHciInit
};

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();
    void handler(void);
    void userNotify(void* pData) {}
private:
    BluetoothStates state;
};

#endif /* BLUETOOTH_H_ */
