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
    hSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    hSpi.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hSpi.Init.TIMode = SPI_TIMODE_DISABLE;
    hSpi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hSpi.Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(&hSpi) == HAL_OK)
    {
        System::getInstance().getConsole()->sendMessage(Severity::Info, name + " initialized");
    }
    else
    {
        System::getInstance().getConsole()->sendMessage(Severity::Error, name + " initialization failed");
    }
    busy = false;
}

SpiBus::~SpiBus()
{
    // TODO Auto-generated destructor stub
}


SpiDevice::SpiDevice(SpiBus* pBus) :
        pBus(pBus)
{
    System::getInstance().getConsole()->sendMessage(Severity::Info, "SPI device created");

}

SpiDevice::~SpiDevice() {}

/*
 * send data to SPI device
 */
void SpiDevice::send(std::vector<uint8_t> data)
{
    dataToSend = data;
    if(HAL_SPI_Transmit_IT(pBus->getHandle(), &dataToSend[0], dataToSend.size()))
    {
        pBus->markAsBusy();
    }
}


/**
  * @brief  Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        // mark this SPI bus as free
        SpiBus::pSpi1->markAsFree();
    }
}
