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

GPIO testPin1(GPIOC, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP); //XXX
GPIO testPin2(GPIOB, GPIO_PIN_1, GPIO_MODE_OUTPUT_PP); //XXX

int main(void)
{
    System::getInstance().config();

    GPIO pushbutton(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    GPIO led(LED2_GPIO_PORT, LED2_PIN, GPIO_MODE_OUTPUT_PP);

    Timer ledTimer;
    // start reception of the first character
    System::getInstance().getConsole()->getInterface().startReception();

    // send first prompt
    System::getInstance().getConsole()->sendPrompt();

    // main loop
    while(1)
    {
        if(ledTimer.elapsed(500000))
        {
            led.toggle();
            ledTimer.reset();
        }

        System::getInstance().getRobot()->getMems().test();
        System::getInstance().getEeprom()->test();

        System::getInstance().getConsole()->handler();
    }

    System::getInstance().terminate();
}
