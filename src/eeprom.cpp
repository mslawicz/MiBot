/*
 * eeprom.cpp
 *
 *  Created on: 12.06.2019
 *      Author: Marcin
 */

#include "eeprom.h"
#include "timer.h"

Eeprom::Eeprom(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
    SpiDevice(pBus, portCS, pinCS)
{

}

Eeprom::~Eeprom()
{
    // TODO Auto-generated destructor stub
}

void Eeprom::test(void)
{
    static Timer timer;
    static uint8_t cnt = 0;
    if(timer.elapsed(10000))
    {
        timer.reset();
        if(++cnt & 0x04)
        {
            send(std::vector<uint8_t>{0xF5, 0x12, 0x34, 0x56});
        }
        else if(cnt & 0x02)
        {
            receiveRequest(10);
        }
        else
        {
            sendReceiveRequest(std::vector<uint8_t>{0xF6, 0x78, 0x9A, 0xBC});
        }
    }
}
