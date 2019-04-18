// @author Rolf "DJ Tendies" Anderson - asaprolfy@gmail.com
//

#ifndef PING_H_
#define PING_H_

#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include <stdbool.h>
#include "Timer.h"
#include "driverlib/interrupt.h"

volatile enum {RISE, FALL, DONE} state;

volatile uint32_t risingEdge;
volatile uint32_t fallingEdge;
volatile uint32_t pulseLength;

void timer_init(void);
void send_pulse(void);
void TIMER3B_Handler(void);
float ping_getDistance(void);
int get_clockcycles(void);
int get_pulsewidth(void);
int get_flows();



struct return_data {
    float distance;
    int clockcycles;
    double pulsewidth;
};

#endif
