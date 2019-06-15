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
    if(timer.elapsed(1300))
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
            read(0x0123, 6);
        }
    }
}

/*
 * read memory content
 */
void Eeprom::read(uint16_t address, uint16_t size)
{
    std::vector<uint8_t> data = {0x03, static_cast<uint8_t>((address >> 8) & 0xFF), static_cast<uint8_t>(address & 0xFF)};
    data.insert(data.end(), size, 0x00);
    sendReceiveRequest(data);
    //XXX should this metho return read data?
}
