/*
 * hci.cpp
 *
 *  Created on: 23.06.2019
 *      Author: Marcin
 */

#include "hci.h"

HCI::HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS) :
    SpiDevice(pBus, portCS, pinCS, false),
    reset(BLUETOOTH_RESET_PORT, BLUETOOTH_RESET_PIN, GPIO_MODE_OUTPUT_PP),
    irq(BLUETOOTH_IRQ_PORT, BLUETOOTH_IRQ_PIN, GPIO_MODE_INPUT, GPIO_PULLDOWN)
{
    state = HciStates::HCIS_start;
    currentWriteBufferSize = 0;
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
        if(irq.read() == GPIO_PinState::GPIO_PIN_SET)
        {
            // IRQ is active, that means there's bluetooth data to read
            state = HCIS_transmit_rd_header;
        }
        break;
    case HCIS_transmit_rd_header:
        select();
        // send read operation code and 4 dummy bytes
        sendReceiveRequest(std::vector<uint8_t>{0x0B, 0x00, 0x00, 0x00, 0x00});
        state = HCIS_wait_for_rd_header_end;
        break;
    case HCIS_wait_for_rd_header_end:
        if(!pBus->isBusy())
        {
            // end of SPI transmission
            state = HCIS_rd_header_received;
        }
        break;
    case HCIS_rd_header_received:
        if((!receptionBuffer.empty()) && (receptionBuffer[0] == 0x02))
        {
            // bluetooth SPI is ready
            currentWriteBufferSize = receptionBuffer[1];
            state = HCIS_check_rd_buffer_size;
        }
        else
        {
            // no valid header received or bluetooth SPI is not ready
            unselect();
            state = HCIS_wait_for_action;
        }
        break;
    case HCIS_check_rd_buffer_size:
        if(receptionBuffer[3] > 0)
        {
            // there's data to read from bluetooth device
            receiveRequest(receptionBuffer[3]);
            state = HCIS_wait_for_rd_buffer_end;
        }
        else
        {
            // no data to read from bluetooth device
            unselect();
            state = HCIS_wait_for_action;
        }
        break;
    case HCIS_wait_for_rd_buffer_end:
        if(!pBus->isBusy())
        {
            // end of SPI transmission
            state = HCIS_rd_buffer_received;
        }
        break;
    case HCIS_rd_buffer_received:
        unselect();//XXX this should be done only when IRQ is inactive
        break;
    default:
        break;
    }
}
