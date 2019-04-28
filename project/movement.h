#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "open_interface.h"
#include "movement.h"
#include "uart.h"
#include "cliffSensor.h"
#include "lcd.h"
#include "robot_position.h"
#include "math.h"

volatile extern int rawDistance;
volatile extern int completion;

void move_forward(oi_t *sensor,int centimeters);

void turn_right(oi_t *sensor,int degrees);

void turn_left(oi_t *sensor,int degrees);

void move_forward_withBumpers(oi_t *sensor,int centimeters);

void move_backward(oi_t *sensor,int centimeters);

int completed();


#endif
