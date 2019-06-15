/*
 * i2c.h
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx_hal.h"
#include <vector>
#include <iostream>

enum DeviceAddress
{
    GyroscopeAdd = 0xD6
};

class I2cDevice;

class I2cBus
{
public:
    I2cBus(I2C_TypeDef* instance);
    ~I2cBus();
    I2C_HandleTypeDef* getHandle(void) const { return const_cast<__I2C_HandleTypeDef*>(&hI2c); }
    void markAsBusy(void) { busy = true; }
    void markAsFree(void) { busy = false; }
    bool isBusy(void) const { return busy; }
    void markNewDataReady(void);
    static I2cBus* pI2c1;
    friend I2cDevice;
private:
    I2C_HandleTypeDef hI2c;
    volatile bool busy;
    I2cDevice* pLastReadDevice;
    std::string name;
};

class I2cDevice
{
public:
    void write(uint8_t registerAddress, std::vector<uint8_t> data);
    void readRequest(uint8_t registerAddress, uint16_t size);
    std::vector<uint8_t> getData(void) { newDataReady = false; return dataBuffer; }
    bool isDataReady(void) const { return newDataReady; }
    friend I2cBus;
protected:
    I2cDevice(I2cBus* pBus, DeviceAddress deviceAddress);
    virtual ~I2cDevice();
private:
    I2cBus* pBus;       // I2C bus for this device
    DeviceAddress deviceAddress;        // I2C device address (7-bit left aligned)
    std::vector<uint8_t> dataBuffer;
    volatile bool newDataReady;         // true if new received data is ready for reading
};

#endif /* I2C_H_ */
