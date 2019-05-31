/*
 * i2c.h
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"

class I2cBus
{
public:
    I2cBus(I2C_TypeDef* instance);
    ~I2cBus();
private:
    I2C_HandleTypeDef hI2c;
};

#endif /* I2C_H_ */
