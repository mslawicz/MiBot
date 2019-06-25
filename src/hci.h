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
#include "timer.h"

#define BLUETOOTH_RESET_PORT GPIOA
#define BLUETOOTH_RESET_PIN  GPIO_PIN_8
#define BLUETOOTH_IRQ_PORT  GPIOA
#define BLUETOOTH_IRQ_PIN   GPIO_PIN_0

enum HciStates
{
    HCIS_start,
    HCIS_reset_on,
    HCIS_reset_wait,
    HCIS_reset_off,
    HCIS_wait_for_action
};

class HCI : public SpiDevice
{
public:
    HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS);
    ~HCI();
    void handler(void);
private:
    HciStates state;
    GPIO reset;
    GPIO irq;
    Timer eventTimer;
    const uint32_t ResetPulseWidth = 1000;
};

#endif /* HCI_H_ */
