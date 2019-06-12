/*
 * eeprom.h
 *
 *  Created on: 12.06.2019
 *      Author: Marcin
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "spi.h"
#include "gpio.h"

class Eeprom : public SpiDevice
{
public:
    Eeprom(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS);
    ~Eeprom();
    void test(void);
};

#endif /* EEPROM_H_ */
