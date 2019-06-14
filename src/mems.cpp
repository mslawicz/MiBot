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
    static bool rw = false;
    if(timer.elapsed(7700))
    {
        timer.reset();
        if(rw)
        {
            gyroscope.readRequest(Lsm6dslRegister::WHO_AM_I, 1 + rand() % 4);
        }
        else
        {
            gyroscope.write(0x73, std::vector<uint8_t>{0x12, 0x34});
        }
        rw = !rw;
    }
    if(gyroscope.isDataReady())
    {
        auto data = gyroscope.getData();
        auto sss = Console::toHex((int)data[0]);
        //System::getInstance().getConsole()->sendMessage(Severity::Info, "I2C received length=" + std::to_string(data.size()) + "  byte=" + Console::toHex((int)data[0], 2));
    }
}
