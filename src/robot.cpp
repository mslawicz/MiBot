/*
 * robot.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "robot.h"

Robot::Robot()
{
    // I2C1 is used for MEMS (X-NUCLEO-IKS01A2) sensors
    pMemsBus = new I2cBus(I2C1);
    pGyroscope = new Gyroscope(pMemsBus->pI2c1, DeviceAddress::GyroscopeAdd);
}

Robot::~Robot()
{
    delete pGyroscope;
    delete pMemsBus;
}

