/*
 * console.h
 *
 *  Created on: 27.05.2019
 *      Author: Marcin
 */

#ifndef CONSOLE_H_
#define CONSOLE_H_

#include "uart.h"

class Console
{
public:
    Console();
    ~Console();
    UART* getInterface(void) const { return pInterface; }
    void handler(void);
    void sendPrompt(void) { pInterface->send(">"); }
private:
    UART* pInterface;
};

#endif /* CONSOLE_H_ */
