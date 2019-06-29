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
#include <vector>
#include <queue>

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
    HCIS_wait_for_action,
    HCIS_transmit_rd_header,
    HCIS_wait_for_rd_header_end,
    HCIS_rd_header_received,
    HCIS_check_rd_buffer_size,
    HCIS_wait_for_rd_buffer_end,
    HCIS_rd_buffer_received,
    HCIS_transmit_wr_header,
    HCIS_wait_for_wr_header_end,
    HCIS_wr_header_received,
    HCIS_check_wr_buffer_size,
    HCIS_wait_for_wr_buffer_end
};

class HCI : public SpiDevice
{
public:
    HCI(SpiBus* pBus, GPIO_TypeDef* portCS, uint32_t pinCS);
    ~HCI();
    void handler(void);
    std::queue<std::vector<uint8_t>>& getEventQueue(void) { return eventQueue; }
    void sendCommand(uint16_t opCode, std::vector<uint8_t> data);
    void sendCommand(uint16_t opCode);
private:
    HciStates state;
    GPIO reset;
    GPIO irq;
    Timer eventTimer;
    const uint32_t ResetPulseWidth = 1000;
    uint8_t currentWriteBufferSize;
    std::queue<std::vector<uint8_t>> eventQueue;
    std::queue<std::vector<uint8_t>> commandQueue;
    std::vector<uint8_t> commandToSend;
};

#endif /* HCI_H_ */
