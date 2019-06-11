/**
  ******************************************************************************
  * @file    main.cpp
  * @author  Marcin
  * @version V1.0
  * @date    22.05.2019
  * @brief   Main MiBot function
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4xx_nucleo.h"
#include "system.h"
#include "gpio.h"
#include "timer.h"
#include "spi.h" //XXX test

GPIO testPin(GPIOC, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP); //XXX

int main(void)
{
    System::getInstance().config();

    GPIO pushbutton(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    GPIO led(LED2_GPIO_PORT, LED2_PIN, GPIO_MODE_OUTPUT_PP);

    SpiBus spi1(SPI1); //XXX
    TestDevice spiTestDevice(&spi1); //XXX

    Timer ledTimer;
    // start reception of the first character
    System::getInstance().getConsole()->getInterface().startReception();

    // send first prompt
    System::getInstance().getConsole()->sendPrompt();

    // main loop
    while(1)
    {
        if(ledTimer.elapsed(100000))
        {
            led.toggle();
            ledTimer.reset();
            spiTestDevice.send(std::vector<uint8_t>{0x12, 0x34, 0x56});
        }

        System::getInstance().getRobot()->getMems().test();

        System::getInstance().getConsole()->handler();
    }

    System::getInstance().terminate();
}
