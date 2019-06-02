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
        gyroscope.write(Lsm6dslRegister::INT1_CTRL, std::vector<uint8_t>{0x12, 0x34});
    }
}
