/*
 * sweep.h
 *
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson
 */

#ifndef SWEEP_H_
#define SWEEP_H_

#include "uart.h"
#include "lcd.h"
#include "inc/tm4c123gh6pm.h"
#include "adc.h"
#include "math.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "string.h"

struct object_info{

    double distance;
    int degree;
    double width;
    //also X position
    //y position
};


void sweep_and_detect_objects();
void checkpoint1_sweep();
void sweep(double x, double y, struct object);

#endif /* SWEEP_H_ */
