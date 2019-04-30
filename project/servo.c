/*
 * servo.c
 * sweep.c
 *
 *  Created on: Mar 29, 2019
 *      Author: zhala
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson, Zane Hala
 */
# include "servo.h"



void servo_init(){

    //pulse_width = 0;

    // turn on clock to PB5
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    // turn on alt func for port b
    GPIO_PORTB_AFSEL_R |= 0x20;

    // turn on t1ccp1 pin
    GPIO_PORTB_PCTL_R |= 0x700000;

    // pb5 output
    GPIO_PORTB_DIR_R |= 0x20;

    // enable pb5 to be digital
    GPIO_PORTB_DEN_R |= 0x20;

    // enable timer 1 clock
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;

    // disable timer 1b to configure
    TIMER1_CTL_R &= ~(TIMER_CTL_TBEN);

    // set it to 16 bit mode
    TIMER1_CFG_R |= 0x00000004;

    // set periodic mode
    TIMER1_TBMR_R |= 0x0A;

    //periodic and PWM enable
    TIMER1_TBMR_R |= (TIMER_TBMR_TBMR_PERIOD | TIMER_TBMR_TBAMS);
    TIMER1_TBMR_R &= ~(TIMER_TBMR_TBCMR);

    //set to non-inverted PWM mode
    TIMER1_CTL_R &= ~(TIMER_CTL_TBPWML);

    // set lower 16 bits of interval register with period
    TIMER1_TBILR_R |= (PERIOD & 0xFFFF);

    // set upper 8 bits of interval register with period
    TIMER1_TBPR_R |= (PERIOD >> 16);

    pulse_width = 0;
    servo_move_to_zero();

    // set lower 16 bits of timer b match with low pulse width
    TIMER1_TBMATCHR_R = ((PERIOD - pulse_width) & 0xFFFF);

    //set upper 8 bits of timer b prescale match with high pulse width
    TIMER1_TBPMR_R = ((PERIOD - pulse_width) >> 16);

    // enable timer 1b after config
    TIMER1_CTL_R |= 0x0100;

    timer_waitMillis(1000);
}

int servo_move(float degree){
    if (degree < 0) {
        return 0;
    }
