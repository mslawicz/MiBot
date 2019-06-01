/*
 * i2c.h
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"

enum DeviceAddress
{
    GyroscopeAdd = 0xD6 >> 1
};

class I2cBus
{
public:
    I2cBus(I2C_TypeDef* instance);
    ~I2cBus();
    static I2C_HandleTypeDef* pI2c1;
private:
    I2C_HandleTypeDef hI2c;
};

class I2cDevice
{
protected:
    I2cDevice(I2C_HandleTypeDef* phI2c, DeviceAddress deviceAddress);
    virtual ~I2cDevice();
private:
    I2C_HandleTypeDef* phI2c;
    DeviceAddress deviceAddress;
};

#endif /* I2C_H_ */
