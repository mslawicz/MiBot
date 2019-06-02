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
    Mems* getMems(void) const { return pMems; }
private:
    Mems* pMems;
};

#endif /* ROBOT_H_ */
