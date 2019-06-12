/*
 * eeprom.cpp
 *
 *  Created on: 12.06.2019
 *      Author: Marcin
 */

#include "eeprom.h"

Eeprom::Eeprom(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
    SpiDevice(pBus, portCS, pinCS)
{

}

Eeprom::~Eeprom()
{
    // TODO Auto-generated destructor stub
}

