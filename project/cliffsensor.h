/*
 * cliffSensor.h
 *
 *  Created on: Apr 16, 2019
 *      Author: Ege Demir
 */

#include "open_interface.h"

#ifndef CLIFFSENSOR_H_
#define CLIFFSENSOR_H_

volatile extern int cliffLeft;
volatile extern int cliffRight;
volatile extern int cliffFrontLeft;
volatile extern int cliffFrontRight;
volatile extern int isBoundary;
volatile extern int on;

int checkCliffSensor(oi_t *sensor);


#endif /* CLIFFSENSOR_H_ */
