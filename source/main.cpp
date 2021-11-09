#include "mbed.h"
#include <iostream>

// main() runs in its own thread in the OS
int main()
{
#ifdef MBED_DEBUG
    HAL_DBGMCU_EnableDBGSleepMode();
#endif

    std::cout << "Hello MiBot!" << std::endl;
    while (true)
    {

    }
}

