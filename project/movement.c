#include "open_interface.h"
#include "movement.h"
#include "uart.h"
#include "cliffSensor.h"
#include "lcd.h"
#include "robot_position.h"
#include "math.h"
#include "sweep.h"

/**
 * movement.c
 *
 * Movement commands, basic functions include moving forward a defined distance, reversing a defined distance, determining boundary crossing, determining cliffs, and sending position data
 * Basis for all movements the robot can make
 * @author Ege Demir, Asa Pauls
 * @date 2/15/2019
 * major revisions started 19 APR 2019
 */

#define CYBOT_TOLERANCE_VALUE_MOVE_FORWARD 0
#define CYBOT_TOLERANCE_VALUE_MOVE_BACKWARD 35
#define CYBOT_TOLERANCE_VALUE_TURN 2

volatile int rawDistance;
volatile int completion;
volatile int sense;

void move_forward(oi_t *sensor, int centimeters) //UPDATED to raw data
{
    completion = 1;
    int left = sensor->leftEncoderCount;

    int bumpL = 0;
    int bumpR = 0;

    int x=0;

    int isBoundaryOrCliff = 0;

    int sum = 0;
    oi_setWheels(100, 100); //move forward
    while (sensor->leftEncoderCount - left < 22.222 * (centimeters) - 32.444)
    {

        // Determines the distance the robot has moved
        oi_update(sensor);
        //Trial code to improve the distance accuracy

        if (left > sensor->leftEncoderCount && x>=5) //If the encoder rolls over during operation movement gets cut off and sum gets spit out without updating
        {
            break;
        }

        sum = 0.045 * (sensor->leftEncoderCount - left) - 1.45998 + 4;

        bumpL = sensor->bumpLeft;
        bumpR = sensor->bumpRight;

        if (bumpL == 1)
        { // decides if the bot has hit something on its left so it responds by backing up and waiting for main logic

            lcd_printf("object cm:%d\n", sum);
            move_backward(sensor, 5);
            sum -= 5;
            sense = 5;
            update_robot_position(sum + 5); //Compensates for the sum -=5 above
            add_object(13);
            update_robot_position(-7); //Compensates for the update_robot_position above
            completion = 0;
            break;
        }

        if (bumpR == 1)
        { // decides if the bot has hit something on its right so it responds by backing up and waiting for main logic
            lcd_printf("object cm:%d\n", sum);
            move_backward(sensor, 5);
            sum -= 5;
            sense = 6;
            update_robot_position(sum + 5); //Compensates for the sum -=5 above
            add_object(13);
            update_robot_position(-7); //Compensates for the update_robot_position above
            completion = 0;
            break;
        }

        isBoundaryOrCliff = checkCliffSensor(sensor);
        if (isBoundaryOrCliff == 1)
        { // decides if the bot has found a hole in the floor or if it has found boundary tape
            move_backward(sensor, sum);
            sum *= -1;
            //turn right or left???????
            update_robot_position(sum);
            completion = 0;
            break;
        }

        x++;
    }
    if (completion == 1) //If the bot was not interrupted it returns cm
    {
        update_robot_position(sum);
    }

    oi_setWheels(0, 0); //stop

}

/*
 * Create 2 and Roomba firmware versions 3.4.0 and earlier return an incorrect value for angle measured in degrees.  The value returned must be divided by 0.324056 to get degrees.
 */
void turn_right(oi_t *sensor, int degrees)
{

    int left = sensor->leftEncoderCount;
    //int right = sensor -> rightEncoderCount; not really necessary would allow more accurate turning, accurate to the point of not relevant REMOVAL?
    oi_setWheels(-100, 100); //Opposing directions allows the robot to just turn on its axis, lower values increase accuracy

    while ((left - sensor->leftEncoderCount) > -5.0771*degrees + 37.91) //sum + CYBOT_TOLERANCE_VALUE_TURN > 0                    rawData = -4.2683*degrees + 27.251    ||  (-4.4355*degrees)+37.599
    {
        oi_update(sensor);
    }

    update_robot_angle(degrees);
    oi_setWheels(0, 0); //stop

}

void move_backward(oi_t *sensor, int centimeters)  //UPDATED to raw data
{
    int left = sensor->leftEncoderCount;

    oi_setWheels(-100, -100); //moves backward

    while (left - sensor->leftEncoderCount < 21.978 * centimeters - 22.769)
    { // Determines the distance the robot has moved
        oi_update(sensor);
    }

    oi_setWheels(0, 0); //stop

}

void turn_left(oi_t *sensor, int degrees)
{

    int left = sensor->leftEncoderCount;

    oi_setWheels(100, -100); //Opposing directions allows the robot to just turn on its axis, lower values increase accuracy

    while ((left - sensor->leftEncoderCount) < ((-4.4355*degrees)+37.599) * -1)
    { // Turns the robot until the angle has been met, the constant corrects the turns
        oi_update(sensor);
    }
    update_robot_angle(-degrees);
    oi_setWheels(0, 0); //stop

}

int completed()
{
    return completion;
}
