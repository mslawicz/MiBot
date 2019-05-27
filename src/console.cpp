/*
 * console.cpp
 *
 *  Created on: 27.05.2019
 *      Author: Marcin
 */

#include "console.h"

Console::Console()
{
    pInterface = new UART(USART2, 115200);
}

Console::~Console()
{
    delete pInterface;
}

/*
 * console handler to be called periodically in a loop
 */
void Console::handler(void)
{
    // handle a received string
    if(pInterface->stringReceived())
    {
        // there is a message in reception queue
        auto message = pInterface->getReceivedString();
        // echo CR LF
        pInterface->send("\r\n");

        // interpret command here
        // XXX send back the string for testing
        pInterface->send("received: '" + message + "'\r\n");

        // send the prompt character
        sendPrompt();
    }

    // handle sending queue
    pInterface->transmitHandler();
}
