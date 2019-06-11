/*
 * spi.h
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx_hal.h"
#include <vector>

class SpiDevice;

class SpiBus
{
public:
    SpiBus(SPI_TypeDef* instance);
    ~SpiBus();
    SPI_HandleTypeDef* getHandle(void) const { return const_cast<__SPI_HandleTypeDef*>(&hSpi); }
    void markAsBusy(void) { busy = true; }
    void markAsFree(void) { busy = false; }
    static SpiBus* pSpi1;
    friend SpiDevice;
private:
    SPI_HandleTypeDef hSpi;
    SPI_TypeDef* instance;
    bool busy;      // true if SPI bus is busy
};

class SpiDevice
{
public:
    void send(std::vector<uint8_t> data);
protected:
    SpiDevice(SpiBus* pBus);
    virtual ~SpiDevice();
private:
    SpiBus* pBus;       // SPI bus for this device
    std::vector<uint8_t> dataToSend;    // vector of data to send
};

// XXX SPI test device
class TestDevice : public SpiDevice
{
public:
    TestDevice(SpiBus* pBus) : SpiDevice(pBus) {}
};

#endif /* SPI_H_ */
