/*
 * mems.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "mems.h"

Gyroscope::Gyroscope(I2C_HandleTypeDef* phI2c, DeviceAddress deviceAddress) :
    I2cDevice(phI2c, deviceAddress)
{

}

