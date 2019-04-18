// @author Rolf "DJ Tendies" Anderson - asaprolfy@gmail.com
//

#include "ping.h"
#include "timer.h"

int numoverflows;

void timer_init(void){

    numoverflows = 0;

    // send clock to GPIO
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    // select alternate 0x08 func for ping sensor
    GPIO_PORTB_AFSEL_R |= 0x08;

    // configure port mux control for pin 3
    GPIO_PORTB_PCTL_R |= 0x7000;

    // digital enable 0x08
    GPIO_PORTB_DEN_R |= 0x08;

    // make 0x08 input
    GPIO_PORTB_DIR_R &= ~0x08;

    // send clock to TIMER3
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    // disable timer3b
    TIMER3_CTL_R &= ~0x100;

    // set 16 bit timer config
    TIMER3_CFG_R |= 0x00000004;

    // input capture mode, edge time count up (0b0111)
    TIMER3_TBMR_R |= 0x17;

    // both edges trigger interrupt
    TIMER3_CTL_R |= 0x0c00;

    // max out 8 bits (24 bit timer now)
    TIMER3_TBPR_R = 0xff;

    // max out load
    TIMER3_TBILR_R = 0xffff;


    // clear timerb interrupt capture
    TIMER3_ICR_R |= 0x400;

    // enable timer3 interrupt
    TIMER3_IMR_R |= 0x400;

    // re-enable timer3b
    TIMER3_CTL_R |= 0x100;

    // set interrupt priority and enable interrupt for timer3b
    NVIC_PRI9_R |= 0x20;
    NVIC_EN1_R |= 0x10;

    // enable interrupt register
    IntRegister(INT_TIMER3B, TIMER3B_Handler);
    IntMasterEnable();
}


void send_pulse(void){

    // enable timer b
    TIMER3_CTL_R &= ~0x300;

    // turn alternate function off
    GPIO_PORTB_AFSEL_R &= ~0x08;

    // set PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;

    // set PB3 to high
    GPIO_PORTB_DATA_R |= 0x08;

    // wait 5 us
    timer_waitMicros(5);

    // set PB3 to low
    GPIO_PORTB_DATA_R &= ~0x08;

    // set PB3 as input
    GPIO_PORTB_DIR_R &= ~0x08;

    // turn alternate function back on
    GPIO_PORTB_AFSEL_R |= 0x08;

    // enable timer b
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){
    if(TIMER3_MIS_R & TIMER_MIS_CBEMIS) {
        if(state == RISE)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R |= TIMER_ICR_CBECINT;
            //captures time of rising edge event
            risingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            //now capturing falling edge
            state = FALL;
        }
        else if(state == FALL)
        {
            //clear capture interrupt flag
            TIMER3_ICR_R |= TIMER_ICR_CBECINT;
            //capture time of falling edge
            fallingEdge = ((int)TIMER3_TBPS_R << 16) | TIMER3_TBR_R;
            state = DONE;
            //disable timerB
            TIMER3_CTL_R &= ~TIMER_CTL_TBEN;

        }
    }
}

float ping_getDistance(void){
    send_pulse();
    //wait until end of pulse
    while(state != DONE)
    {

    }

    state = RISE;

    //Correct for overflow
    int overflow = (fallingEdge < risingEdge);

    if(overflow == 1){
        numoverflows++;
    }

    pulseLength = fallingEdge + (overflow<<24) - risingEdge;

    //calculate distance in mm
    return pulseLength / 903.2;
}

int get_flows(){
    return numoverflows;
}

int get_pulsewidth(void){
    return pulseLength;
}



/* void send_pulse(void){
    // Turn alt functions off
    GPIO_PORTB_AFSEL_R &= ~(0x08);
    // Turn off port
    GPIO_PORTB_PCTL_R &= ~(0x0000F000);
    // Set PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;
    // set PB3 high
    GPIO_PORTB_DATA_R &= 0x08;
    timer_waitMicros(5);
    // Set PB3 low
    GPIO_PORTB_DATA_R |= 0xF7;
    state = RISE;
    // Set PB3 as input
    GPIO_PORTB_DIR_R &= 0xF7;
    // set pb3 gpio commit register
    GPIO_PORTB_CR_R |= 0x08;
    // Alt functions on
    GPIO_PORTB_AFSEL_R |= 0x80;
    // Enable timer function
    GPIO_PORTB_PCTL_R |= 0x00007000;
    // Enable timer and start counting
    TIMER3_CTL_R |= 0x3;
}
 */
/*
 *  GPIO_PORTB_AFSEL_R |= 0x80;
    GPIO_PORTB_DEN_R &= 0xF7;
    GPIO_PORTB_PDR_R &= 0xF7;
    GPIO_PORTB_PUR_R &= 0xF7;
    GPIO_PORTB_CR_R |= 0x08;
 */

/*
 * void timer_init(void){
    // Turn on clock to timer 3
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R3;

    // Turn on port b
    SYSCTL_RCGCGPIO_R |= 0x02;

    // Pin 3 enable
    GPIO_PORTB_DEN_R |= 0x08;
    GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB3_T3CCP1;

    // Configure timer for input capture
    TIMER3_CTL_R |= 0xD00;     //~TIMER_CTL_TBEN;

    // Edge select
    //TIMER3_CTL_R |= TIMER_CTL_TBEVENT_BOTH;

    // 16 bit counter
    TIMER3_CFG_R |= 0x4; //TIMER_CFG_16_BIT;

    // Capture mode
    TIMER3_TBMR_R = 0x17;   //TIMER_TBMR_TBMR_CAP;

    // Edge timer mode
    //TIMER3_TBMR_R |= TIMER_TBMR_TBCMR;

    // Count up
    //TIMER3_TBMR_R |= TIMER_TBMR_TBCDIR;

    // Set upper bounds
    TIMER3_TBILR_R |= 0xFFFF;

    // Enable capture
    TIMER3_IMR_R |= TIMER_IMR_CBEIM;

    // Clear interrupts
    TIMER3_ICR_R |= TIMER_ICR_CBECINT;

    // Set up interrupts
    // interrupt 36
    NVIC_PRI9_R |= NVIC_PRI9_INT36_M;
    NVIC_EN1_R |= 0x10;

    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    //TIMER3_CTL_R |= TIMER_CTL_TBEN;
}
 */
