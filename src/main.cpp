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

int main(void)
{
    System::getInstance()->config();


    System::getInstance()->terminate();
}
