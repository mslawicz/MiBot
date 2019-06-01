/*
 * console.cpp
 *
 *  Created on: 27.05.2019
 *      Author: Marcin
 */

#include "console.h"
#include <unordered_map>

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
        sendMessage(Severity::Debug, "received: '" + message + "'");

        // send the prompt character
        sendPrompt();
    }

    // handle sending queue
    pInterface->transmitHandler();
}

/*
 * send log message to console
 */
void Console::sendMessage(Severity level, std::string message)
{
    const std::unordered_map<Severity, std::string> severityStrings =
    {
            {Error, "error"},
            {Warning, "warning"},
            {Info, "info"},
            {Debug, "debug"}
    };
    message = severityStrings.find(level)->second + ": " + message + "\r\n";
    pInterface->send(message);
}
