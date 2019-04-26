/*
 * cliffSensor.c
 *
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
volatile int on;
#define BOUNDARY_DETECTION 2700


int checkCliffSensor(oi_t *sensor)
{
    while(on==1){
    oi_update(sensor);

    cliffLeft = sensor -> cliffLeftSignal;
    cliffRight = sensor -> cliffRightSignal;
    cliffFrontLeft = sensor -> cliffFrontLeftSignal;
    cliffFrontRight = sensor -> cliffFrontRightSignal;

    if(cliffLeft>BOUNDARY_DETECTION||cliffRight>BOUNDARY_DETECTION||cliffFrontLeft>BOUNDARY_DETECTION||cliffFrontRight>BOUNDARY_DETECTION)
        return 1;


    if(cliffLeft<1100||cliffRight<1100||cliffFrontLeft<1100||cliffFrontRight<1100)
    {
        return 1;
    }

    }
    return 0;
}
