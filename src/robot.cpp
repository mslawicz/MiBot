/*
 * robot.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "robot.h"

Robot::Robot()
{
    pMemsBus = new I2cBus(I2C1);

}

Robot::~Robot()
{
    delete pMemsBus;
}

