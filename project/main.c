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
#include "sound.h"

int main(){
    lcd_init();
    servo_init();
    uart_init();
    timer_init();
    adc_init();


    while(1){
        sweep();
        timer_waitMillis(50);
        driver_main();

    }
}

