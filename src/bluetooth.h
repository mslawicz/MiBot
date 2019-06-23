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
    BTS_start
};

class Bluetooth
{
public:
    Bluetooth();
    ~Bluetooth();
    void handler(void);
private:
    BluetoothStates state;
};

#endif /* BLUETOOTH_H_ */
