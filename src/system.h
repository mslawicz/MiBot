/*
 * system.h
 *
 *  Created on: 22.05.2019
 *      Author: Marcin
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "uart.h"
#include "console.h"
#include "robot.h"

class System
{
public:
    ~System();
    static System* getInstance(void);
    void errorHandler(void) {};
    void config(void);
    void terminate(void);
    Console* getConsole(void) const { return pConsole; }
private:
    System();
    void configClock(void);
    Console* pConsole;
    Robot* pRobot;
};

#endif /* SYSTEM_H_ */
