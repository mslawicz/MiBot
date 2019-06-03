/*
 * mems.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "mems.h"
#include "system.h"//XXX
#include "console.h"//XXX

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
    if(timer.elapsed(500000))
    {
        timer.reset();
        gyroscope.readRequest(Lsm6dslRegister::WHO_AM_I, 1);
    }
    if(gyroscope.isDataReady())
    {
        auto data = gyroscope.getData();
        System::getInstance().getConsole()->sendMessage(Severity::Info, "I2C received length=" + std::to_string(data.size()) + "  byte=" + std::to_string((int)data[0]));
    }
}
