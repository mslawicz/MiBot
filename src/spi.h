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

#define EEPROM_CS_PORT GPIOA
#define EEPROM_CS_PIN  GPIO_PIN_9
#define BLUETOOTH_CS_PORT GPIOA
#define BLUETOOTH_CS_PIN  GPIO_PIN_1

class SpiDevice;

class SpiBus
{
public:
    SpiBus(SPI_TypeDef* instance);
    ~SpiBus();
    SPI_HandleTypeDef* getHandle(void) const { return const_cast<__SPI_HandleTypeDef*>(&hSpi); }
    DMA_HandleTypeDef* getDmaRxHandle(void) const { return const_cast<DMA_HandleTypeDef*>(&hDmaSpi1Rx); }
    DMA_HandleTypeDef* getDmaTxHandle(void) const { return const_cast<DMA_HandleTypeDef*>(&hDmaSpi1Tx); }
    void markAsBusy(void) { busy = true; }
    void markAsFree(void);
    void markNewDataReady(void);
    bool isBusy(void) const { return busy; }
    static SpiBus* pSpi1;
    friend SpiDevice;
private:
    SPI_HandleTypeDef hSpi;
    SPI_TypeDef* instance;
    bool busy;      // true if SPI bus is busy
    SpiDevice* pLastServedDevice;
    DMA_HandleTypeDef hDmaSpi1Rx;
    DMA_HandleTypeDef hDmaSpi1Tx;
};

class SpiDevice
{
public:
    void send(std::vector<uint8_t> data);
    void receiveRequest(uint16_t size);
    void sendReceiveRequest(std::vector<uint8_t> data);
    void select(void) { chipSelect.write(GPIO_PinState::GPIO_PIN_RESET); }
    void unselect(void) { chipSelect.write(GPIO_PinState::GPIO_PIN_SET); }
    friend SpiBus;
protected:
    SpiDevice(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS, bool autoCS = true);
    virtual ~SpiDevice();
    SpiBus* pBus;       // SPI bus for this device
    std::vector<uint8_t> receptionBuffer;    // vector of data being received
private:
    std::vector<uint8_t> dataToSend;    // vector of data to send
    GPIO chipSelect;
    bool newDataReady;
    bool autoCS;
};

#endif /* SPI_H_ */
