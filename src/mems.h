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
    INT1_CTRL = 0x0D
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
    I2cBus* getBus(void) const { return pBus; }
    void test(void);    //XXX
private:
    I2cBus* pBus;
    Gyroscope* pGyroscope;
    Timer timer;
};



#endif /* MEMS_H_ */
