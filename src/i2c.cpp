/*
 * i2c.cpp
 *
 *  Created on: 31.05.2019
 *      Author: Marcin
 */

#include "i2c.h"
#include "gpio.h"

I2cBus::I2cBus(I2C_TypeDef* instance)
{
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
    if (HAL_I2C_Init(&hI2c) != HAL_OK)
    {
      //ret = HAL_ERROR;
    }
    HAL_I2CEx_ConfigAnalogFilter(&hI2c, I2C_ANALOGFILTER_ENABLE);
}

I2cBus::~I2cBus()
{
    // TODO Auto-generated destructor stub
}

