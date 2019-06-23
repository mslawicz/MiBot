/*
 * hci.cpp
 *
 *  Created on: 23.06.2019
 *      Author: Marcin
 */

#include "hci.h"

HCI::HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
    SpiDevice(pBus, portCS, pinCS)
{
    // TODO Auto-generated constructor stub

}

HCI::~HCI()
{
    // TODO Auto-generated destructor stub
}

/*
 * send HCI command to bluetooth device
 */
void HCI::sendCommand(uint16_t ogf, uint16_t ocf, std::vector<uint8_t>data)
{
    uint16_t opCode = (ocf << 6) | ogf;
    uint8_t dataLength = static_cast<uint8_t>(data.size());
    data.insert(data.begin(), {static_cast<uint8_t>((opCode >> 8) & 0xFF), static_cast<uint8_t>(opCode & 0xFF), dataLength});
    send(data);
}
