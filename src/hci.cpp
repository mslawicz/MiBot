/*
 * hci.cpp
 *
 *  Created on: 23.06.2019
 *      Author: Marcin
 */

#include "hci.h"

HCI::HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
    SpiDevice(pBus, portCS, pinCS),
    reset(BLUETOOTH_RESET_PORT, BLUETOOTH_RESET_PIN, GPIO_MODE_OUTPUT_PP),
    irq(BLUETOOTH_IRQ_PORT, BLUETOOTH_IRQ_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN)
{
    state = HciStates::HCIS_start;
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
    switch(state)
    {
    case HCIS_start:
        state = HCIS_reset_on;
        break;
    case HCIS_reset_on:     // switch on reset signal
        reset.write(GPIO_PinState::GPIO_PIN_RESET);
        eventTimer.reset();
        state = HCIS_reset_wait;
        break;
    case HCIS_reset_wait:   // wait for the end of the reset pulse
        if(eventTimer.elapsed(ResetPulseWidth))
        {
            reset.write(GPIO_PinState::GPIO_PIN_SET);
            state = HCIS_reset_off;
        }
        break;
    case HCIS_reset_off:    // after reset pulse
        state = HCIS_wait_for_action;
        break;
    case HCIS_wait_for_action:  // wait for reception or send request
        break;
    default:
        break;
    }
}
