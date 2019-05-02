/*
 * cliffSensor.c
 *deteccts cliffs and boundary tape
 *  Created on: Apr 16, 2019
 *      Author: Ege Demir
 */

#include "open_interface.h"
#include "cliffSensor.h"

volatile int cliffLeft;
volatile int cliffRight;
volatile int cliffFrontLeft;
volatile int cliffFrontRight;
volatile int isBoundary;
volatile int cliffhigh;
volatile int boundary;
volatile int sense;
#define BOUNDARY_DETECTION 2700

/*
 * The purpose of 'on' is so that the logic that determines the goal allows it to overide the movement command
 * that stops the robot from passing over the tape it determines to indicate a boundary. This allows the robot to enter
 * the destination.
 * 
 */
int checkCliffSensor(oi_t *sensor)
{

    oi_update(sensor);

    cliffLeft = sensor->cliffLeftSignal;
    cliffRight = sensor->cliffRightSignal;
    cliffFrontLeft = sensor->cliffFrontLeftSignal;
    cliffFrontRight = sensor->cliffFrontRightSignal;

    cliffhigh = cliffLeft;
    boundary=cliffLeft;
    sense = 1;

    /*
     * Sense descriptions
     * sense is so that the robot has a easy way to determine which sensor has been activated
     * A '1' indicates the left most sensor
     * A '2' indicates the front left sensor
     * A '3' indicates the front right sensor
     * A '4' indicates the right most sensor
     */

    if (cliffLeft > BOUNDARY_DETECTION || cliffRight > BOUNDARY_DETECTION || cliffFrontLeft > BOUNDARY_DETECTION || cliffFrontRight > BOUNDARY_DETECTION)
    {
        if (cliffhigh < cliffFrontLeft)
        {
            cliffhigh = cliffFrontLeft;
            sense = 2;
        }
        else if (cliffhigh < cliffFrontRight)
        {
            cliffhigh = cliffFrontRight;
            sense = 3;
        }
        else if (cliffhigh < cliffRight)
        {
            cliffhigh = cliffRight;
            sense = 4;
        }
        return 1;
    }

    if (cliffLeft < 1100 || cliffRight < 1100 || cliffFrontLeft < 1100 || cliffFrontRight < 1100)
    {
        if (boundary > cliffFrontLeft)
        {
            boundary = cliffFrontLeft;
            sense = 2;
        }
        else if (boundary > cliffFrontRight)
        {
            boundary = cliffFrontRight;
            sense = 3;
        }
        else if (boundary > cliffRight)
        {
            boundary = cliffRight;
            sense = 4;
        }
        return 1;
    }

    return 0;
}
