/*
 * robot.cpp
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#include "robot.h"

Robot::Robot()
{
    pMems = new Mems;
}

Robot::~Robot()
{
    delete pMems;
}

