#include "open_interface.h"
#include "movement.h"
#include "uart.h"
#include "cliffSensor.h"
#include "lcd.h"
#include "robot_position.h"
#include "math.h"

/**
 * movement.c
 *
 * Basic movement commands
 * Basis for all movements the robot can make
 * @author Ege Demir, Asa Pauls
 * @date 2/15/2019
 */

#define CYBOT_TOLERANCE_VALUE_MOVE_FORWARD -60
#define CYBOT_TOLERANCE_VALUE_MOVE_BACKWARD 35
#define CYBOT_TOLERANCE_VALUE_TURN 2

/*
 * Notes on how the encoder increments
 *The cumulative number of raw left encoder counts is returned as a signed 16-bit number,
 *The  high byte first. This number will roll over if it passes the max value (at approx. 14.5 meters).
 *Range: -32768 - 32767 counts
 *NOTE: These encoders are square wave, not quadrature, so they rely on the robot’s commanded velocity direction to know when to count up/down.
 *So if the robot is trying to drive forward, and you force the iRobot® Roomba Open Interface (OI) Specification iRobot to spin in reverse,
 *the encoders will count up, (and vice-versa).  Additionally, the encoders will count up when the commanded velocity is zero and the wheels spin.
 *To convert counts to distance, simply do a unit conversion using the equation for circle circumference.
 *N counts * (mm in 1 wheel revolution / counts in 1 wheel revolution) = mm
 *N counts * (pi * 72.0 / 508.8) = mm
 */

void move_forward(oi_t *sensor,int centimeters)
{
    int x=0;
    int isBoundaryOrCliff = 0;

    int milli=(centimeters*10) + CYBOT_TOLERANCE_VALUE_MOVE_FORWARD;
    int sum = 0;
    oi_setWheels(200,200); //move forward full speed
    while(sum < milli)
    {
        // Determines the distance the robot has moved
        oi_update(sensor);
        update_robot_position((((sensor -> distance)*(M_PI*72))/509)*0.4545); //Trial code to improve the distance accuracy
        //update_robot_position(((sensor -> distance)/4));

        isBoundaryOrCliff = checkCliffSensor(sensor);
        if(isBoundaryOrCliff==1 && x>=3)
        {
            move_backward(sensor,5);
            turn_right(sensor,180);
            x=0;
            break;
        }

        x++;
        sum += sensor -> distance;

    }

    oi_setWheels(0,0); //stop
}

/*
 * Create 2 and Roomba firmware versions 3.4.0 and earlier return an incorrect value for angle measured in degrees.  The value returned must be divided by 0.324056 to get degrees.
 */
void turn_right(oi_t *sensor,int degrees)
{
    int sum=degrees;

    oi_setWheels(-100,100); //Opposing directions allows the robot to just turn on its axis


    while(sum + CYBOT_TOLERANCE_VALUE_TURN > 0)
    { // Turns the robot until the angle has been met, the constant corrects the turns

        oi_update(sensor);
        sum += sensor -> angle;
        lcd_printf("sum:%d\nsenVal:%d",sum,sensor -> angle);
    }

    update_robot_angle(degrees);
    oi_setWheels(0,0); //stop

}





void move_backward(oi_t *sensor,int centimeters)
{
        int milli=(centimeters*-10) + CYBOT_TOLERANCE_VALUE_MOVE_BACKWARD;
        int sum = 0;
        oi_setWheels(-200,-200); //move forward full speed

        while(sum > milli) { // Determines the distance the robot has moved
            oi_update(sensor);
            update_robot_position(-((sensor -> distance)/6));
            sum += sensor -> distance;
        }


        oi_setWheels(0,0); //stop


}

void move_forward_withBumpers(oi_t *sensor,int centimeters)
{
    int milli=centimeters*10;
    int sum = 0;

    int bumpL = 0;
    int bumpR= 0;
     //move forward full speed

    while(sum < milli) {
        oi_setWheels(300,300);
        oi_update(sensor);
        sum += sensor -> distance; // increments sum with the distance it has travelled since last update



            bumpL= sensor -> bumpLeft;
            bumpR= sensor -> bumpRight;


            if(bumpL==1){ // decides if the bot has hit something on its left so it responds by backing up, turning right, facing forward, then resuming travel
                move_backward(sensor,15);
                sum=sum-250;
                turn_right(sensor,-90);
                move_forward(sensor,25);
                turn_left(sensor,90);
            }


            if(bumpR==1){ // decides if the bot has hit something on its right so it responds by backing up, turning left, facing forward, then resuming travel
                    move_backward(sensor,15);
                    sum=sum-250;
                    turn_left(sensor,90);
                    move_forward(sensor,25);
                    turn_right(sensor,-90);

                }

    }


    oi_setWheels(0,0); //stop


}




void turn_left(oi_t *sensor,int degrees)
{

    oi_setWheels(200,-200); //Opposing directions allows the robot to just turn on its axis
    int sum=0;
    int sumDegrees=0;

    while(sumDegrees<degrees+5){ // Turns the robot until the angle has been met, the constant corrects the turns

        oi_update(sensor);
        sum+=sensor -> angle;
        sumDegrees=sum;


    }

    oi_setWheels(0,0); //stop

}





