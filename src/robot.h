/*
 * robot.h
 *
 *  Created on: 01.06.2019
 *      Author: Marcin
 */

#ifndef ROBOT_H_

#include "mems.h"

class Robot
{
public:
    Robot();
    ~Robot();
    Mems& getMems(void) { return mems; }
private:
    Mems mems;
};

#endif /* ROBOT_H_ */
