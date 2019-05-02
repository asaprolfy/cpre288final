/*
 * l10_main.c
 *
 *  Created on: Apr 16, 2019
 *      Author: bsj1
 */
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include "lcd.h"
#include "timer.h"
#include "servo.h"
#include "button.h"
#include "uart.h"
#include "ping.h"
#include "adc.h"
#include "movement.h"
#include "open_interface.h"
#include "geometry.h"
#include "robot_position.h"
#include "sweep.h"
#include "driving.h"


int main(){
    lcd_init();
    servo_init();
    uart_init();
    timer_init();
    adc_init();
    int num_objects;
    double x, y, width = 0;
    int part = 5;

    //test to see if you can detect the smallest object and print the position
    if(part == 1){
        oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);

        servo_move(0);
        timer_waitMillis(50);
        sweep();

        num_objects = get_num_objects();
        get_object_info(&x, &y, &width, num_objects-1);

        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", num_objects, x, y, width);

        oi_free(sensor_data);
    }
    //this allows you to test the driving through objects function
    if(part == 2){
        while(1){
            sweep();
            /*
            num_objects = get_num_objects();
            get_object_info(&x, &y, &width, num_objects);

            lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", num_objects, x, y, width);
        */
            timer_waitMillis(50);
            driver_main();

        }
    }
    //test to see if robot is calibrated right
    //drive in a square
    if(part == 3){
        oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        /*
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_left(sensor_data, 90);
        move_forward(sensor_data, 20);
*/
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 20);
        turn_right(sensor_data, 90);
        move_forward(sensor_data, 20);

        oi_free(sensor_data);


    }

    if(part == 4)
    {
        oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        move_forward(sensor_data,40);

        oi_free(sensor_data);
    }

    if (part == 5){
        sweep();
        lcd_printf("%d", get_num_objects());
        timer_waitMillis(3000);
        sweep();
        lcd_printf("%d", get_num_objects());

        while(1){
            int i;
            double x,y,width;
            int size = get_num_objects();
            for(i=0; i < get_num_objects(); i++){

                get_object_info(&x,&y,&width, i);
                lcd_printf("%d %.1lf %.1lf",i, x, y);
                timer_waitMillis(3000);

            }
        }

    }


    while(1){

    }

}

