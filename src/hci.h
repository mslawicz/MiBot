/*
 * hci.h
 *
 *  Created on: 23.06.2019
 *      Author: Marcin
 */

#ifndef HCI_H_
#define HCI_H_

#include "spi.h"
#include "gpio.h"

class HCI : public SpiDevice
{
public:
    HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS);
    ~HCI();
    void handler(void);
};

#endif /* HCI_H_ */
