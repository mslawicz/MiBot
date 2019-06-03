/*
 * mems.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "mems.h"

Mems::Mems() :
    // MEMS (X-NUCLEO-IKS01A2) sensors use I2C1 bus
    bus(I2C1),
    gyroscope(&bus, DeviceAddress::GyroscopeAdd)
{
}

Mems::~Mems()
{
}

Gyroscope::Gyroscope(I2cBus* pBus, DeviceAddress deviceAddress) :
    I2cDevice(pBus, deviceAddress)
{

}

void Mems::test(void)
{
    if(timer.elapsed(10000))
    {
        timer.reset();
        gyroscope.readRequest(Lsm6dslRegister::WHO_AM_I, 1);
    }
}
