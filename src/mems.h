/*
 * mems.h
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#ifndef MEMS_H_
#define MEMS_H_

#include "i2c.h"
#include "timer.h"//XXX

enum Lsm6dslRegister
{
    INT1_CTRL = 0x0D,
    WHO_AM_I = 0x0F
};

class Gyroscope : public I2cDevice
{
public:
    Gyroscope(I2cBus* pBus, DeviceAddress deviceAddress);
};

class Mems
{
public:
    Mems(void);
    ~Mems(void);
    I2cBus& getBus(void) { return bus; }
    void test(void);    //XXX
private:
    I2cBus bus;
    Gyroscope gyroscope;
    Timer timer;
};



#endif /* MEMS_H_ */
