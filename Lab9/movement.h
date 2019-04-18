#include "open_interface.h"

/*
 * movement.h
 *
 *  Created on: Feb 8, 2019
 *      Author: zhala
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

void move_forward(oi_t *sensor_data, int centimeters);
void move_backward(oi_t *sensor_data, int centimeters);
void turn_right(oi_t *sensor_data, int degrees);
void turn_left(oi_t *sensor_data, int degrees);
int checkBump(oi_t *sensor_data);

#endif /* MOVEMENT_H_ */
