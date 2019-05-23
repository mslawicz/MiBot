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

int main(void)
{
    System::getInstance()->config();

    GPIO pushbutton(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN, GPIO_MODE_INPUT, GPIO_PULLUP);
    GPIO led(LED2_GPIO_PORT, LED2_PIN, GPIO_MODE_OUTPUT_PP);
    while(1)
    {
        uint32_t delay;
        if(pushbutton.read())
        {
            delay = 850;
        }
        else
        {
            delay = 450;
        }
        led.write(GPIO_PinState::GPIO_PIN_SET);
        HAL_Delay(100);
        led.write(GPIO_PinState::GPIO_PIN_RESET);
        HAL_Delay(100);
        led.write(GPIO_PinState::GPIO_PIN_SET);
        HAL_Delay(100);
        led.write(GPIO_PinState::GPIO_PIN_RESET);
        HAL_Delay(delay);
    }

    System::getInstance()->terminate();
}
