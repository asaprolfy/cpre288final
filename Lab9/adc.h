#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void adc_init(void);
void adc_read(double distance);
int adc_get_raw(void);
int adc_avg_distance();
