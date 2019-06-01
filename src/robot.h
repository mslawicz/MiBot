/*
 * robot.h
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include "i2c.h"
#include "mems.h"

class Robot
{
public:
    Robot();
    ~Robot();
private:
    I2cBus* pMemsBus;
    Gyroscope* pGyroscope;
};

#endif /* ROBOT_H_ */
