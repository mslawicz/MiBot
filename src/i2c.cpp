/*
 * i2c.cpp
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#include "i2c.h"
#include "gpio.h"
#include "system.h"

I2C_HandleTypeDef* I2cBus::pI2c1 = nullptr;

I2cBus::I2cBus(I2C_TypeDef* instance)
{
    std::string name;
    if(instance == I2C1)
    {
        // SCL pin
        GPIO(GPIOB, GPIO_PIN_8, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF4_I2C1);
        // SDA pin
        GPIO(GPIOB, GPIO_PIN_9, GPIO_MODE_AF_OD, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF4_I2C1);
        /* Peripheral clock enable */
        __HAL_RCC_I2C1_CLK_ENABLE();
        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 8, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
        pI2c1 = &hI2c;
        name = "I2C1";
    }
    hI2c.Instance = instance;
    hI2c.Init.ClockSpeed = 400000;
    hI2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hI2c.Init.OwnAddress1 = 0;
    hI2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hI2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hI2c.Init.OwnAddress2 = 0;
    hI2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hI2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hI2c) == HAL_OK)
    {
        System::getInstance()->getConsole()->sendMessage(Severity::Info, name + " initialized");
    }
    else
    {
        System::getInstance()->getConsole()->sendMessage(Severity::Error, name + " initialization failed");
    }
    if(HAL_I2CEx_ConfigAnalogFilter(&hI2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        System::getInstance()->getConsole()->sendMessage(Severity::Error, name + " filter configuration failed");
    }
}

I2cBus::~I2cBus()
{
    // TODO Auto-generated destructor stub
}

I2cDevice::I2cDevice(I2C_HandleTypeDef* phI2c, DeviceAddress deviceAddress) :
        phI2c(phI2c),
        deviceAddress(deviceAddress)
{

}

I2cDevice::~I2cDevice() {}

/*
 * write data to I2C device
 */
void I2cDevice::write(uint8_t registerAddress, std::vector<uint8_t> data)
{
    // copy data to buffer
    dataBuffer = data;
    HAL_I2C_Mem_Write_IT(phI2c, deviceAddress, registerAddress, I2C_MEMADD_SIZE_8BIT, &dataBuffer[0], dataBuffer.size());
}
