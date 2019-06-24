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
 * HCI state machine to handle SPI transactions
 * to be executed periodically in a loop
 */
void HCI::handler(void)
{

}
