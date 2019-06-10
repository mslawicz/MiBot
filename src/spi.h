/*
 * spi.h
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx_hal.h"

class SpiBus
{
public:
    SpiBus(SPI_TypeDef* instance);
    ~SpiBus();
    SPI_HandleTypeDef* getHandle(void) const { return const_cast<__SPI_HandleTypeDef*>(&hSpi); }
    static SpiBus* pSpi1;
private:
    SPI_HandleTypeDef hSpi;
    SPI_TypeDef* instance;
};

class SpiDevice
{
public:
    SpiDevice();
    ~SpiDevice();
};

#endif /* SPI_H_ */
