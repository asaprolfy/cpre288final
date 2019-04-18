#include "adc.h"
#include "timer.h"

void adc_init(void){
    // GPIO
    SYSCTL_RCGCGPIO_R = SYSCTL_RCGCGPIO_R | 0b000010;

    GPIO_PORTB_AFSEL_R |= 0x10;

    GPIO_PORTB_DIR_R &= 0b10111111;

    GPIO_PORTB_DEN_R &= 0b11101111;

    GPIO_PORTB_AMSEL_R |= 0x10;

    GPIO_PORTB_ADCCTL_R = 0x00;

    // ADC
    SYSCTL_RCGCADC_R |= 0x1;

    GPIO_PORTB_ADCCTL_R = 0x0;
    // This wait has to be here or it won't run
    timer_waitMillis(300);

    ADC0_ACTSS_R &= 0xE;

    ADC0_EMUX_R &= 0xFFF0;

    ADC0_SSMUX0_R |= 0x0000000A;

    ADC0_SSCTL0_R = 0x000000006;

    ADC0_SAC_R  = ADC_SAC_AVG_32X;

    ADC0_ACTSS_R |= 0x1;

    // Hardware over sampling pg 807 datasheet
    //
}

void adc_read(double distance){
    ADC0_PSSI_R = 0x1;

    timer_waitMillis(300);
}

int adc_get_raw(void){
    ADC0_PSSI_R = 0x1;
    while((ADC0_RIS_R & 0x1) == 0){

    }
    ADC0_ISC_R = 0x01;
    return ADC0_SSFIFO0_R;
}

int adc_avg_distance(){
    int avg[20];
    int i;
    int sum = 0;

    for(i = 0; i < 20; i++){
        avg[i] = adc_get_raw();
        sum += avg[i];
//        avg = (avg * (i - 1) + adc_get_raw())/i;
    }

    return sum/20;

}
