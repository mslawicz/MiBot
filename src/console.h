/*
 * console.h
 *
 *  Created on: 27.05.2019
 *      Author: Marcin
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "uart.h"

enum Severity
{
    Error,
    Warning,
    Info,
    Debug
};

class Console
{
public:
    Console();
    ~Console();
    UART* getInterface(void) const { return pInterface; }
    void handler(void);
    void sendPrompt(void) { pInterface->send(">"); }
    void sendMessage(Severity level, std::string message);
private:
    UART* pInterface;
};

#endif /* CONSOLE_H_ */
