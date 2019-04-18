/*
 * sweep.h
 *
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson
 */

#include "uart.h"
#include "lcd.h"
#include "inc/tm4c123gh6pm.h"
#include "adc.h"
#include "math.h"
#include "ping.h"
#include "servo.h"
#include "button.h"
#include "string.h"

void sweep_and_detect_objects();
void checkpoint1_sweep();
