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
    UART& getInterface(void) { return interface; }
    void handler(void);
    void sendPrompt(void) { interface.send(">"); }
    void sendMessage(Severity level, std::string message);
private:
    UART interface;
};

#endif /* CONSOLE_H_ */
