#include "open_interface.h"
#include "movement.h"
/*
 * movement.c
 *
 *  Created on: Feb 8, 2019
 *      Author: zhala
 */


void move_forward(oi_t *sensor_data, int centimeters) {

    int sum = 0;

    oi_setWheels(100,100);

    while(sum < centimeters*10) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
        //lcd_printf("%d", sum);
    }
    oi_setWheels(0,0); //stop

}

void move_backward(oi_t *sensor_data, int centimeters) {

    int sum = centimeters*10;

    oi_setWheels(-200,-200);

    while(sum > 0) {
        oi_update(sensor_data);
        sum += sensor_data -> distance;
        //lcd_printf("%d", sum);
    }
    oi_setWheels(0,0); //stop

}

void turn_right(oi_t *sensor_data, int degrees){

    int sum = degrees;

    oi_setWheels(-100, 100);

    while(sum > 0){
        oi_update(sensor_data);
        sum += sensor_data -> angle;
        //lcd_printf("%d", sum);
    }

    oi_setWheels(0, 0);
}

void turn_left(oi_t *sensor_data, int degrees){

    int sum = 0;

    oi_setWheels(100, -100);

    while(sum < degrees){
        oi_update(sensor_data);
        sum += sensor_data -> angle;
        //lcd_printf("%d", sum);
    }

    oi_setWheels(0, 0);
}

int checkBump(oi_t *sensor_data){
    if (sensor_data -> bumpLeft){
        return 1;
    }
    else if (sensor_data -> bumpRight){
        return 2;
    }
    else if (sensor_data -> bumpRight && sensor_data -> bumpLeft){
        return 3;
    }

    return 0;
}



