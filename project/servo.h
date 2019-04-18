/*
 * servo.h
 *
 *  Created on: Mar 29, 2019
 *      Author: zhala
 */

#ifndef SERVO_H_
#define SERVO_H_

#define PERIOD 320000

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"
#include "button.h"
#include "lcd.h"

volatile signed pulse_width;

struct object_info{

    double distance;
    int degree;
    double width;
    //also X position
    //y position
};

void servo_init();
int servo_move(float degrees);
void servo_move_to_zero();
void checkpoint2();

#endif /* SERVO_H_ */
