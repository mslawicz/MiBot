/*
 * i2c.h
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"
#include "gpio.h"//XXX
#include <vector>

enum DeviceAddress
{
    GyroscopeAdd = 0xD6
};

class I2cBus
{
public:
    I2cBus(I2C_TypeDef* instance);
    ~I2cBus();
    I2C_HandleTypeDef* getHandle(void) const { return const_cast<__I2C_HandleTypeDef*>(&hI2c); }
    void markAsBusy(void) { busy = true; testPin.write(GPIO_PIN_SET);} //XXX
    void markAsFree(void) { busy = false; testPin.write(GPIO_PIN_RESET);} //XXX
    static I2cBus* pI2c1;
private:
    I2C_HandleTypeDef hI2c;
    volatile bool busy;
    GPIO testPin;//XXX
};

class I2cDevice
{
public:
    void write(uint8_t registerAddress, std::vector<uint8_t> data);
    //void read(uint8_t registerAddress, std::vector<uint8_t>& data);
protected:
    I2cDevice(I2cBus* pBus, DeviceAddress deviceAddress);
    virtual ~I2cDevice();
private:
    I2cBus* pBus;       // I2C bus for this device
    DeviceAddress deviceAddress;        // I2C device address (7-bit right aligned)
    std::vector<uint8_t> dataBuffer;
};

#endif /* I2C_H_ */
