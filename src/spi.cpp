/*
 * spi.cpp
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#include "spi.h"
#include "system.h"

SpiBus* SpiBus::pSpi1 = nullptr;

SpiBus::SpiBus(SPI_TypeDef* instance) :
    instance(instance)
{
    std::string name;
    if(instance == SPI1)
    {
        name = "SPI1";

        // MISO pin
        GPIO(GPIOA, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        // MOSI pin
        GPIO(GPIOA, GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        // SCK pin
        GPIO(GPIOB, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_VERY_HIGH, GPIO_AF5_SPI1);
        /* Peripheral clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();
        /* Peripheral interrupt init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 1, 1);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);

        /* DMA interrupt init */
        /* DMA2_Stream0_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
        /* DMA2_Stream3_IRQn interrupt configuration */
        HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);

        /* SPI1_RX Init */
        hDmaSpi1Rx.Instance = DMA2_Stream0;
        hDmaSpi1Rx.Init.Channel = DMA_CHANNEL_3;
        hDmaSpi1Rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hDmaSpi1Rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hDmaSpi1Rx.Init.MemInc = DMA_MINC_ENABLE;
        hDmaSpi1Rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hDmaSpi1Rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hDmaSpi1Rx.Init.Mode = DMA_NORMAL;
        hDmaSpi1Rx.Init.Priority = DMA_PRIORITY_LOW;
        hDmaSpi1Rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hDmaSpi1Rx) != HAL_OK)
        {
            System::getInstance().getConsole()->sendMessage(Severity::Error, name + " RX DMA initialization failed");
        }
        __HAL_LINKDMA(&hSpi,hdmarx,hDmaSpi1Rx);

        /* SPI1_TX Init */
        hDmaSpi1Tx.Instance = DMA2_Stream3;
        hDmaSpi1Tx.Init.Channel = DMA_CHANNEL_3;
        hDmaSpi1Tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hDmaSpi1Tx.Init.PeriphInc = DMA_PINC_DISABLE;
        hDmaSpi1Tx.Init.MemInc = DMA_MINC_ENABLE;
        hDmaSpi1Tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hDmaSpi1Tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hDmaSpi1Tx.Init.Mode = DMA_NORMAL;
        hDmaSpi1Tx.Init.Priority = DMA_PRIORITY_LOW;
        hDmaSpi1Tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
        if (HAL_DMA_Init(&hDmaSpi1Tx) != HAL_OK)
        {
            System::getInstance().getConsole()->sendMessage(Severity::Error, name + " TX DMA initialization failed");
        }
        __HAL_LINKDMA(&hSpi,hdmatx,hDmaSpi1Tx);

        pSpi1 = this;
    }
    hSpi.Instance = instance;
    hSpi.Init.Mode = SPI_MODE_MASTER;
    hSpi.Init.Direction = SPI_DIRECTION_2LINES;
    hSpi.Init.DataSize = SPI_DATASIZE_8BIT;
    hSpi.Init.CLKPolarity = SPI_POLARITY_LOW;
    hSpi.Init.CLKPhase = SPI_PHASE_1EDGE;
    hSpi.Init.NSS = SPI_NSS_SOFT;
    hSpi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;    // clock 666 kHz
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
    pLastServedDevice = nullptr;
}

SpiBus::~SpiBus()
{
    // TODO Auto-generated destructor stub
}

/*
 * mark the SPI bus inactive and release chip select of the current device
 */
void SpiBus::markAsFree(void)
{
    busy = false;
    if(pLastServedDevice != nullptr)
    {
        // current served device chip select inactive
        pLastServedDevice->chipSelect.write(GPIO_PinState::GPIO_PIN_SET);
    }
}


SpiDevice::SpiDevice(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
        pBus(pBus),
        chipSelect(portCS, pinCS, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_VERY_HIGH)
{
    System::getInstance().getConsole()->sendMessage(Severity::Info, "SPI device created");
    chipSelect.write(GPIO_PinState::GPIO_PIN_SET);
    newDataReady = false;
}

SpiDevice::~SpiDevice() {}

/*
 * send data to SPI device
 */
void SpiDevice::send(std::vector<uint8_t> data)
{
    dataToSend = data;
    chipSelect.write(GPIO_PinState::GPIO_PIN_RESET);
    if(HAL_SPI_Transmit_DMA(pBus->getHandle(), &dataToSend[0], dataToSend.size()) == HAL_OK)
    {
        pBus->markAsBusy();
        pBus->pLastServedDevice = this;
    }
    else
    {
        // no transmission started
        chipSelect.write(GPIO_PinState::GPIO_PIN_SET);
        pBus->pLastServedDevice = nullptr;
    }
}

/*
 * receive data from SPI device
 */
void SpiDevice::receiveRequest(uint16_t size)
{
    receptionBuffer.assign(size, 0);
    chipSelect.write(GPIO_PinState::GPIO_PIN_RESET);
    if(HAL_SPI_Receive_DMA(pBus->getHandle(), &receptionBuffer[0], size) == HAL_OK)
    {
        pBus->markAsBusy();
        pBus->pLastServedDevice = this;
    }
    else
    {
        // no reception started
        chipSelect.write(GPIO_PinState::GPIO_PIN_SET);
        pBus->pLastServedDevice = nullptr;
    }
}

/*
 * bidirectional transmit to/from SPI device
 */
void SpiDevice::sendReceiveRequest(std::vector<uint8_t> data)
{
    dataToSend = data;
    receptionBuffer.assign(data.size(), 0);
    chipSelect.write(GPIO_PinState::GPIO_PIN_RESET);
    if(HAL_SPI_TransmitReceive_DMA(pBus->getHandle(), &dataToSend[0], &receptionBuffer[0], data.size()) == HAL_OK)
    {
        pBus->markAsBusy();
        pBus->pLastServedDevice = this;
    }
    else
    {
        // no reception started
        chipSelect.write(GPIO_PinState::GPIO_PIN_SET);
        pBus->pLastServedDevice = nullptr;
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


/**
  * @brief  Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        // mark this SPI bus as free
        SpiBus::pSpi1->markAsFree();
        SpiBus::pSpi1->markNewDataReady();
    }
}

/**
  * @brief  Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if(hspi->Instance == SPI1)
    {
        // mark this SPI bus as free
        SpiBus::pSpi1->markAsFree();
        SpiBus::pSpi1->markNewDataReady();
    }
}

/*
 * mark that new received data is ready in the device buffer
 */
void SpiBus::markNewDataReady(void)
{
    if(pLastServedDevice != nullptr)
    {
        pLastServedDevice->newDataReady = true;
    }
}

/**
  * @brief  SPI error callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *               the configuration information for SPI module.
  * @retval None
  */
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    System::getInstance().getConsole()->sendMessage(Severity::Error, " SPI error code=" + std::to_string(HAL_SPI_GetError(hspi)));
}
