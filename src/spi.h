/*
 * spi.h
 *
 *  Created on: 10.06.2019
 *      Author: Marcin
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx_hal.h"
#include "gpio.h"
#include <vector>

class SpiDevice;

class SpiBus
{
public:
    SpiBus(SPI_TypeDef* instance);
    ~SpiBus();
    SPI_HandleTypeDef* getHandle(void) const { return const_cast<__SPI_HandleTypeDef*>(&hSpi); }
    void markAsBusy(void) { busy = true; }
    void markAsFree(void);
    void markNewDataReady(void);
    static SpiBus* pSpi1;
    friend SpiDevice;
private:
    SPI_HandleTypeDef hSpi;
    SPI_TypeDef* instance;
    bool busy;      // true if SPI bus is busy
    SpiDevice* pLastServedDevice;
};

class SpiDevice
{
public:
    void send(std::vector<uint8_t> data);
    void receiveRequest(uint16_t size);
    friend SpiBus;
protected:
    SpiDevice(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS);
    virtual ~SpiDevice();
private:
    SpiBus* pBus;       // SPI bus for this device
    std::vector<uint8_t> dataToSend;    // vector of data to send
    std::vector<uint8_t> receptionBuffer;    // vector of data being received
    GPIO chipSelect;
    bool newDataReady;
};

#endif /* SPI_H_ */
