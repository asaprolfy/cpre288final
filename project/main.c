#include "uart.h"
#include "lcd.h"
#include "inc/tm4c123gh6pm.h"
#include "adc.h"
#include "math.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "string.h"
#include "sweep.h"
/**
 * main.c
 */

struct object_info{

    double distance;
    int degree;
    double width;
    //also X position
    //y position
};


int main(){
    lcd_init();
    timer_init();
    adc_init();
    servo_init();
    uart_init();

    checkpoint1_sweep();

    zanes_main_logic();
}

