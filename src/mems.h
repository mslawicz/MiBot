/*
 * mems.h
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#ifndef MEMS_H_
#define MEMS_H_

#include "i2c.h"

class Gyroscope : public I2cDevice
{
public:
    Gyroscope(I2cBus* pBus, DeviceAddress deviceAddress);
};

#endif /* MEMS_H_ */
