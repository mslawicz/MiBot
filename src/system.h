/*
 * system.h
 *
 *  Created on: 22.05.2019
 *      Author: Marcin
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

class System
{
public:
    ~System();
    static System* getInstance(void);
    void errorHandler(void) {};
    void config(void);
    void terminate(void);
private:
    System();
    void configClock(void);
};

#endif /* SYSTEM_H_ */
