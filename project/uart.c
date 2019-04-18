#include "uart.h"
#include "driverlib/interrupt.h"

volatile int *rx_event_ptr;
volatile char *rx_char_ptr;

#define BIT0        0x01
#define BIT1        0x02
#define BIT2        0x04
#define BIT3        0x08
#define BIT4        0x10
#define BIT5        0x20
#define BIT6        0x40
#define BIT7        0x80

void uart_init_int(volatile int *rx_event_addr, volatile char *rx_char_addr){
    rx_event_ptr = rx_event_addr;
    rx_char_ptr = rx_char_addr;

    //enable clock to GPIO on port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //enable clock to UART1
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

    //enable alternate functions on port B pins 0 and 1
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);

    //enable Rx and Tx on port B on pins 0 and 1
    GPIO_PORTB_PCTL_R |= 0x00000011;

    //set pin 0, 1, 2
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1 | BIT2);

    //set pin 0 to Rx or input
    GPIO_PORTB_DIR_R &= ~BIT0;

    //set pin 1 to Tx or output
    GPIO_PORTB_DIR_R |= BIT1;

    //calculate baudrate
    // iBRD = 104, fBRD = 11 for 9600 baud
    // iBRD = 8, fBRD = 44 for 115200 baud

    uint16_t iBRD = 8;
    uint16_t fBRD = 44;

    //turn off uart1
    UART1_CTL_R &= ~(UART_CTL_UARTEN);

    //set baud rate
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
    UART1_LCRH_R = UART_LCRH_WLEN_8 ;

    //use system clock as source
    UART1_CC_R = UART_CC_CS_SYSCLK;

    //Setup interrupts
    UART1_ICR_R |= (BIT5 | BIT4);
    UART1_IM_R |= (BIT5 | BIT4);

    NVIC_EN0_R |= 0x00000040;
    NVIC_PRI1_R |= 0x00200000;

    IntRegister(INT_UART1, uart_handler);

    IntMasterEnable();
    //re-enable enable RX, TX, and uart1
    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

}

void uart_init(){
    //enable clock to GPIO on port B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;

    //enable clock to UART1
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;

    //enable alternate functions on port B pins 0 and 1
    GPIO_PORTB_AFSEL_R |= (BIT0 | BIT1);

    //enable Rx and Tx on port B on pins 0 and 1
    GPIO_PORTB_PCTL_R |= 0x00000011;

    //set pin 0, 1, 2
    GPIO_PORTB_DEN_R |= (BIT0 | BIT1 | BIT2);

    //set pin 0 to Rx or input
    GPIO_PORTB_DIR_R &= ~BIT0;

    //set pin 1 to Tx or output
    GPIO_PORTB_DIR_R |= BIT1;

    //calculate baudrate
    // iBRD = 104, fBRD = 11 for 9600 baud
    // iBRD = 8, fBRD = 44 for 115200 baud

    uint16_t iBRD = 8;
    uint16_t fBRD = 44;

    //turn off uart1
    UART1_CTL_R &= ~(UART_CTL_UARTEN);

    //set baud rate
    UART1_IBRD_R = iBRD;
    UART1_FBRD_R = fBRD;

    //set frame, 8 data bits, 1 stop bit, no parity, no FIFO
    UART1_LCRH_R = UART_LCRH_WLEN_8 ;

    //use system clock as source
    UART1_CC_R = UART_CC_CS_SYSCLK;

    UART1_CTL_R = (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);
}

void uart_handler() {

    // Clear interrupt status register
    if(UART1_MIS_R & BIT4){
        UART1_ICR_R |= BIT4;
        *rx_event_ptr = 1;
        *rx_char_ptr = uart_receive();
    }
    else if(UART1_MIS_R & BIT5){
            UART1_ICR_R |= BIT5;
    }
}


void uart_sendChar(char data){
    //wait until there is room to send data
    while(UART1_FR_R & 0x20)
    {

    }
    //send data
    UART1_DR_R = data;
}

char uart_receive(void){
    char data = 0;
    //wait to receive
    while(UART1_FR_R & UART_FR_RXFE)
    {

    }
    //mask the 4 error bits and grab only 8 data bits
    data = (char)(UART1_DR_R & 0xFF);
    return data;

}

void display_from_putty(void){
    char data[20];
    char input = 0;
    int i = 0;
    lcd_printf("%s", "waiting");
    while(1){
        input = uart_receive();
        if(input != '\r'){
            data[i] = input;
            i++;
            lcd_printf("%c %d", input, i);
        }
        if(input == '\r' || i >= 20){
            lcd_printf("%s", data);
        }
    }
}

void echo_to_putty(void){
    char input = 0;
    int i = 0;
    lcd_printf("%s", "waiting");
    while(1){
        input = uart_receive();
        if(input != '\r'){
            i++;
            lcd_printf("%c %d", input, i);
            uart_sendChar(input);
        }
        if(input == '\r'){
            uart_sendChar('\r');
            uart_sendChar('\n');
        }
    }
}

void uart_sendStr(char str[30]) {
    int i;

    for(i = 0; str[i] != '\0'; i++) {
        uart_sendChar(str[i]);
    }
    for(; i < 30; i++) {
        uart_sendChar(' ');
    }

    uart_sendChar('\n');
}

void uart_sendStr_gayVersion(const char *data)
{
    //until we reach a null character
    while (*data != '\0')
    {
        //send the current character
        uart_sendChar(*data);
        // increment the pointer.
        data++;
    }
}
