/*
 * sweep.c
 *
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson, Zane Hala
 */

#include "sweep.h"
#include "uart.h"
#include "adc.h"
#include <math.h>
#include "ping.h"
#include "servo.h"
#include "geometry.h"
#include "robot_position.h"
#include "cliffSensor.h"

object_info object[50];
int object_count = 0;

// Don't delete this please gamers
// Sweeps for objects and returns an integer of the amount of objects detected
void sweep()
{
    int count = object_count;
    int degrees, start, finish, flag = 0;
    double ir, sonar;
    double x_obj, y_obj;
    int i, mid_angle;

    for (i = 0; i < 181; i++)
    {
        degrees = i;
        sonar = ping_getDistance();
        // specific to bot 6
        ir = pow(adc_avg_distance(), -.885) * 22239;

        // if there is something within range begin keeping track of degrees
        if (abs(sonar - ir) < 20 && flag == 0 && sonar < 60)
        {
            flag = 1;
            start = degrees;
        }
        // if the object is no longer detected calculate range and size, add to struct array
        if (abs(sonar - ir) > 20 && flag == 1 && sonar < 60 && ir > sonar)
        {
            finish = degrees;
            flag = 0;
            // put in the struct given
            mid_angle = (start + finish) / 2;
            object_position(mid_angle, sonar, &x_obj, &y_obj);
            object[count].distance = sonar;
            object[count].width = sqrt(
                    2 * pow(sonar, 2)
                            - 2 * pow(sonar, 2)
                                    * cos((finish - start) * (M_PI / 180))) - 3;
            object[count].x = x_obj;
            object[count].y = y_obj + (object[count].width / 2);
            count++;
        }
        servo_move(i);

    }

    object_count += count;

}

int get_num_objects()
{
    return object_count;
}

void get_object_info(double *x, double *y, double *width, int index)
{

    *x = object[index].x;
    *y = object[index].y;
    *width = object[index].width;
}

void add_object(double width)
{ //Removed the x,y function parameters and moved robot x,y to being called in the function (Asa)
    double x,y;
    get_robot_position(&x,&y);
    object_count += 1;
    object[object_count].x = x;
    object[object_count].y = y;
    object[object_count].width = width;
    double angle = get_robot_angle(); //Allows for the direction the robot is moving to be realized (Asa)
    /*
     *  sense allows the robot to determine on which side of the robot is the object it hit. '5' left side, '6' is the right side.
     * Somewhat arbitrary distances are based off a sketch of a 45 degree path in the quadrant, applies to all the "math" (Asa)
     *
     */
    if (sense == 5)
    {
        if (angle > 0 && angle < 90) //Q1
        {
            object[object_count].x = x;
            object[object_count].y = y + 5;
        }
        else if (angle >= 90 && angle < 180) //Q2
        {
            object[object_count].x = x-5;
            object[object_count].y = y;
        }
        else if (angle >= 180 && angle < 270) //Q3
        {
            object[object_count].x = x;
            object[object_count].y = y - 5;
        }
        else if (angle >= 270 && (angle <= 360 || angle == 0)) //Q4
        {
            object[object_count].x = x + 5;
            object[object_count].y = y;
        }
        object[object_count].width = 13;
    }
    else if (sense == 6)
    {
        if (angle > 0 && angle <= 90) //Q1
        {
            object[object_count].x = x + 5;
            object[object_count].y = y;
        }
        else if (angle > 90 && angle <= 180) //Q2
        {
            object[object_count].x = x;
            object[object_count].y = y + 5;
        }
        else if (angle > 180 && angle <= 270) //Q3
        {
            object[object_count].x = x-5;
            object[object_count].y = y;
        }
        else if (angle > 270 && (angle <= 360 || angle == 0)) //Q4
        {
            object[object_count].x = x;
            object[object_count].y = y-5;
        }
        object[object_count].width = 13;
    }
}
