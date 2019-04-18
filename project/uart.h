#include <stdint.h>
#include <stdbool.h>
#include "timer.h"
#include "lcd.h"
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

void uart_init_int(volatile int *rx_event_addr, volatile char *rx_char_addr);
void uart_init();
void uart_sendChar(char data);
char uart_receive(void);
void display_from_putty();
void echo_to_putty();
void uart_handler();
void uart_sendStr_gayVersion(const char *data);
void uart_sendStr(char str[]);
