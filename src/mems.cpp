/*
 * mems.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "mems.h"

Mems::Mems()
{
    // MEMS (X-NUCLEO-IKS01A2) sensors use I2C1 bus
    pBus = new I2cBus(I2C1);
    pGyroscope = new Gyroscope(pBus, DeviceAddress::GyroscopeAdd);
}

Mems::~Mems()
{
    delete pGyroscope;
    delete pBus;
}

Gyroscope::Gyroscope(I2cBus* pBus, DeviceAddress deviceAddress) :
    I2cDevice(pBus, deviceAddress)
{

}

