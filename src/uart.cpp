/*
 * uart.cpp
 *
 *  Created on: 27.05.2019
 *      Author: Marcin
 */

#include "uart.h"
#include "system.h"
#include "gpio.h"

UART_HandleTypeDef* UART::pUSART2 = nullptr;

UART::UART(USART_TypeDef* instance, uint32_t baudRate) :
    instance(instance)
{
    if(instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        //UART TX pin
        GPIO(GPIOA, GPIO_PIN_9, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1);
        //UART RX pin
        GPIO(GPIOA, GPIO_PIN_10, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1);
    }
    if(instance == USART2)
    {
        __HAL_RCC_USART2_CLK_ENABLE();
        //UART TX pin
        GPIO(GPIOA, GPIO_PIN_2, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART2);
        //UART RX pin
        GPIO(GPIOA, GPIO_PIN_3, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART2);
    }
    if(instance == USART6)
    {
        __HAL_RCC_USART6_CLK_ENABLE();
        //UART TX pin
        GPIO(GPIOC, GPIO_PIN_6, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF8_USART6);
        //UART RX pin
        GPIO(GPIOC, GPIO_PIN_7, GPIO_MODE_AF_PP, GPIO_PULLUP, GPIO_SPEED_FREQ_LOW, GPIO_AF8_USART6);
    }

    hUart.Instance = instance;
    hUart.Init.BaudRate = baudRate;
    hUart.Init.WordLength = UART_WORDLENGTH_8B;
    hUart.Init.StopBits = UART_STOPBITS_1;
    hUart.Init.Parity = UART_PARITY_NONE;
    hUart.Init.Mode = UART_MODE_TX_RX;
    hUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    hUart.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&hUart) != HAL_OK)
    {
      System::getInstance().errorHandler();
    }

    if(instance == USART2)
    {
        pUSART2 = &hUart;
        /* USART2 interrupt Init */
        HAL_NVIC_SetPriority(USART2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(USART2_IRQn);
    }
    busy = false;
    receptionBuffer = 0;
}

UART::~UART()
{
    if(instance == USART2)
    {
        /* USART2 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
}


/**
  * @brief  Tx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        System::getInstance().getConsole()->getInterface().clearBusyFlag();
    }
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART2)
    {
        // move received character to the reception string
        System::getInstance().getConsole()->getInterface().moveReceivedByte();
        // wait for the next character to receive
        System::getInstance().getConsole()->getInterface().startReception();
    }
}

/*
 * UART transmit handler to be called periodically in a loop
 */
void UART::transmitHandler(void)
{
    if((!busy) && (!transmitQueue.empty()))
    {
        // UART is not busy and the transmit queue is not empty
        stringToSend = transmitQueue.front();
        transmitQueue.pop();
        busy = true;
        HAL_UART_Transmit_IT(&hUart, reinterpret_cast<uint8_t*>(&stringToSend[0]), stringToSend.length());
    }
}

/*
 * move received character to reception string
 * store complete string to the reception queue
 * to be called in HAL_UART_RxCpltCallback
 */
void UART::moveReceivedByte(void)
{
    if(receptionBuffer == 0x0A)
    {
        // LF character received
        receptionQueue.push(receivedString);
        receivedString.clear();
    }
    else
    {
        // any other character received
        receivedString.push_back(receptionBuffer);
        // send echo
        send(std::string(1, receptionBuffer));
    }
}

/*
 * get received string from the reception queue
 */
std::string UART::getReceivedString(void)
{
    __HAL_UART_DISABLE_IT(&hUart, UART_IT_RXNE);
    auto str = receptionQueue.front();
    receptionQueue.pop();
    __HAL_UART_ENABLE_IT(&hUart, UART_IT_RXNE);
    return str;
}

