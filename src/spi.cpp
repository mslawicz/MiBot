/*
 * spi.cpp
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#include "spi.h"
#include "gpio.h"
#include "system.h"

SpiBus* SpiBus::pSpi1 = nullptr;

SpiBus::SpiBus(SPI_TypeDef* instance) :
    instance(instance)
{
    std::string name;
    if(instance == SPI1)
    {
        // MISO pin
        GPIO(GPIOA, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        // MOSI pin
        GPIO(GPIOA, GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        // SCK pin
        GPIO(GPIOB, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();
        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 7, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
        pSpi1 = this;
        name = "SPI1";
    }
    hSpi.Instance = instance;
    hSpi.Init.Mode = SPI_MODE_MASTER;
    hSpi.Init.Direction = SPI_DIRECTION_2LINES;
    hSpi.Init.DataSize = SPI_DATASIZE_8BIT;
    hSpi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hSpi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hSpi.Init.NSS = SPI_NSS_SOFT;
    hSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hSpi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hSpi.Init.TIMode = SPI_TIMODE_DISABLE;
    hSpi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hSpi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hSpi) != HAL_OK)
    {
        System::getInstance().getConsole()->sendMessage(Severity::Info, name + " initialized");
    }
    else
    {
        System::getInstance().getConsole()->sendMessage(Severity::Error, name + " initialization failed");
    }

}

SpiBus::~SpiBus()
{
    // TODO Auto-generated destructor stub
}


SpiDevice::SpiDevice()
{
    // TODO Auto-generated constructor stub

}

SpiDevice::~SpiDevice()
{
    // TODO Auto-generated destructor stub
}
