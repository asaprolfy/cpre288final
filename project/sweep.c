/*
 * sweep.c
 *
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson
 */

#include "sweep.h"


void sweep_and_detect_objects() {

    struct object_info object[3];
    int degrees, start, finish, flag = 0;
    double ir, sonar;
    int count = 0;
    int i;
    // sweep
    for(i = 0; i < 181; i++){
        degrees = i;
        sonar = ping_getDistance();
//        ir = 105375 * pow(adc_avg_distance(), -1.153);
        ir = pow(adc_avg_distance(), -1.855) * 2E+07;

        // if there is something within range begin keeping track of degrees
        if(abs(sonar-ir) < 20 && flag == 0 && sonar < 60){
            flag = 1;
            start = degrees;
            object[count].distance = sonar; //nick and I found it is better to measure distance at start of the sweep
            //count++;
        }
        // if the object is no longer detected calculate range and size, add to struct array
        if(abs(sonar-ir) > 20 && flag == 1 && sonar <60 && ir>sonar){
            finish = degrees;
            flag = 0;
            //object[count].distance = sonar;
            object[count].degree = finish-start;
            object[count].width = sqrt(2*pow(sonar,2)-2*pow(sonar,2)*cos((finish-start)*(M_PI/180))) - 3;
            count++;
            printf("%d\t%d\n", start, finish);
        }
        servo_move(i);

    }
    lcd_printf("\ndistance\tnum_degree\twidth\n");
    // try to determine smallest object
    int smallest_index = 0;
    double smallest_width = 0;
    for(i=0; i<count; i++){
        if((smallest_width == 0) && (object[i].width > 1)){
            smallest_width = object[i].width;
        }
        if((object[i].width < smallest_width) && (object[i].width > 1)){
            smallest_width = object[i].width;
            smallest_index = i;
        }
    }
    // if smallest was found print it
    if(count){
        lcd_printf("Dist: %.2f width degrees: %d width cm: %.2f count: %d smallest: %d", object[smallest_index].distance, object[smallest_index].degree, object[smallest_index].width, count, smallest_index+1);
    }
    else{
        lcd_printf("No objects found");
    }



    // attempt to send to putty
    uart_sendStr("\ndistance\tnum_degree\twidth\n");
}


void checkpoint1_sweep() {

    //struct object_info c1_objects[10];
    int j;
    int c1_degrees;
    //int currently_detecting_obj = 0;
    //int obj_num = 0;

    double ir_distance, sonar_distance;

    //uart_sendStr("Degrees    IR_Dist     Sonar_Dist\n");

    lcd_printf("Degrees\nIR_Dist\nSonar_Dist\n");
    uart_sendStr("Degrees, IR_Dist, Sonar_Dist");

    // sweep and send each degree to putty
    for(j = 0; j < 181; j += 2) {
        c1_degrees = j;

        //ir_distance = pow(adc_avg_distance(), -1.855) * 2E+07;
        ir_distance = 105375 * pow(adc_avg_distance(), -1.153);
        sonar_distance = ping_getDistance();

        lcd_printf("%d\n%f\n%f\n", c1_degrees, ir_distance, sonar_distance);
        char str[30];
        sprintf(str, "%d, %f, %f", c1_degrees, ir_distance, sonar_distance);
        uart_sendStr(str);

        servo_move(j);

        timer_waitMillis(200);
    }

    servo_move(0);
}

void sweep(double x, double y, struct object){
    int degrees, start, finish, flag = 0;
    double ir, sonar;
    double *x_obj, *y_obj
    int count = 0;
    int i, mid_angle;
    for(i = 0; i < 181; i++){
        degrees = i;
        sonar = ping_getDistance();
        ir = pow(adc_avg_distance(), -1.855) * 2E+07;

        // if there is something within range begin keeping track of degrees
        if(abs(sonar-ir) < 20 && flag == 0 && sonar < 60){
            flag = 1;
            start = degrees;
        }
        // if the object is no longer detected calculate range and size, add to struct array
        if(abs(sonar-ir) > 20 && flag == 1 && sonar <60 && ir>sonar){
            finish = degrees;
            flag = 0;
            // put in the struct given
            mid_angle = (start + finish) / 2;
            object_position(x, y, mid_angle, sonar, *x_obj, *y_obj;
            object[count].distance = sonar;
            object[count].width = sqrt(2*pow(sonar,2)-2*pow(sonar,2)*cos((finish-start)*(M_PI/180))) - 3;
            object[count].x = *x_obj;
            object[count].y = y_obj
            count++;
        }
        servo_move(i);

    }
}

