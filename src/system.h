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
#include "spi.h"
#include "eeprom.h"
#include "bluetooth.h"

class System
{
public:
    ~System();
    static System& getInstance(void);
    void errorHandler(void) {};
    void config(void);
    void terminate(void);
    Console* getConsole(void) const { return pConsole; }
    Robot* getRobot(void) const { return pRobot; }
    Eeprom* getEeprom(void) const { return pEeprom; }
    Bluetooth* getBluetooth(void) const { return pBluetooth; }
private:
    System();
    void configClock(void);
    SpiBus* pSpi1;
    Eeprom* pEeprom;
    Console* pConsole;
    Robot* pRobot;
    Bluetooth* pBluetooth;
};

#endif /* SYSTEM_H_ */
