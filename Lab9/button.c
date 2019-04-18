/*
 *  button.c
 *  @author: Rolf Anderson
 *  @date: 02/22/2019
 *
 *
 */

#include "button.h"

volatile int *button_event_ptr;
volatile int *button_num_ptr;

// The buttons are on PORTE 0:5
// GPIO_PORTE_DATA_R -- the register that must be checked on port E to determine which buttons are being pressed

#define BIT0		0x01
#define BIT1		0x02
#define BIT2		0x04
#define BIT3		0x08
#define BIT4		0x10
#define BIT5		0x20
#define BIT6		0x40
#define BIT7		0x80

uint8_t prev_Button; //must be set yourself in button_getButton()
uint8_t button; //current button being pressed, must be set yourself in button_getButton()

/**
 * Initialize PORTE and configure bits 0-5 to be used as inputs for the buttons.
 */
void button_init() {
	static uint8_t initialized = 0;

	//Check if already initialized
	if(initialized){
		return;
	}

	// To initialize and configure GPIO PORTE, visit pg. 656 in the Tiva datasheet.
	// Follow steps in 10.3 for initialization and configuration. Some steps have been outlined below.
	// Ignore all other steps in initialization and configuration that are not listed below. You will learn more
	// about additional steps in a later lab.

	//Turn on PORTE system clock
	SYSCTL_RCGCGPIO_R |= 0b010000;

	//Set the buttons to inputs and enable
	GPIO_PORTE_DEN_R |= 0xFF;
	GPIO_PORTE_DIR_R &= 0b1000000;


	initialized = 1;
}

/**
 * Interrupt handler -- executes when a hardware event occurs (a button is pressed)
 */
void init_button_interrupts(volatile int *button_event_addr, volatile int *button_num_addr) {

    button_event_ptr = button_event_addr;
    button_num_ptr = button_num_addr;

    // In order to configure GPIO ports to detect interrupts, you will need to visit pg. 656 in the Tiva datasheet.
    // Notice that you already followed some steps in 10.3 for initialization and configuration of the GPIO ports in the function button_init().
    // Additional steps for setting up the GPIO port to detect interrupts have been outlined below.
    // TO DO: Complete code below

    // Mask the bits for pins 0-5
    GPIO_PORTE_IM_R &= 0b11000000;

    // Set pins 0-5 to use edge sensing
    GPIO_PORTE_IS_R &= 0b11000000;

    // Set pins 0-5 to use both edges. We want to update the LCD
    // when a button is pressed, and when the button is released.
    GPIO_PORTE_IBE_R |= 0b00111111;

    // Clear the interrupts
    GPIO_PORTE_ICR_R = 0b00111111;

    // Unmask the bits for pins 0-5
    GPIO_PORTE_IM_R |= 0b00111111;

    // TO DO: Complete code below
    // Enable GPIO port E interrupt
    NVIC_EN0_R |= 0b00000000000000000000000000010000;

    // Bind the interrupt to the handler.
    IntRegister(INT_GPIOE, gpioe_handler);
}

/**
 * Handles a hardware interrupt that occurs from a button being pressed
 */
void gpioe_handler() {

    // Clear interrupt status register
     GPIO_PORTE_ICR_R |= 0b00111111;

    *button_event_ptr = 1;
    *button_num_ptr = button_getButton();
    //lcd_printf("%d", button_num);
}

/**
 * returns a 6-bit bit field, representing the push buttons. A 1 means the corresponding button is pressed.
 */
uint8_t button_checkButtons()
{
    return (~GPIO_PORTE_DATA_R) & (BIT6 - 1); //Return the button status
}

/**
 * Returns the position of the leftmost button being pushed.
 * @return the position of the leftmost button being pushed. A 6 is the leftmost button, 1 is the rightmost button. Return 0 if no button is being pressed.
 */
uint8_t button_getButton() {

	//
	// DELETE ME - How bitmasking works
	// ----------------------------------------
	// In embedded programming, often we only care about one or a few bits in a piece of 
	// data. There are several bitwise operators that we can apply to data in order
	// to "mask" the bits that we don't care about.
	//
	//	| = bitwise OR		& = bitwise AND		^ = bitwise XOR		~ = bitwise NOT
	//		  << x = shift left by x bits		 >> x = shift right by x bits 
	//
	// Let's say we want to know if push button 3 (SW3) of GPIO_PORTE_DATA_R is
	// pushed.  Since push buttons are high (1) initially, and low (0) if pushed, PORTE should
	// look like:
	// GPIO_PORTE_DATA_R => 0b???? ?0?? if SW3 is pushed
	// GPIO_PORTE_DATA_R => 0b???? ?1?? if SW3 is not pushed
	//	
	// Bitwise AND:
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0000 if SW3 is pushed
	// (GPIO_PORTE_DATA_R & 0b0000 0100) => 0b0000 0100 if SW3 is not pushed
	//
	// Bitwise OR:
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1011 if SW3 is pushed
	// (GPIO_PORTE_DATA_R | 0b1111 1011) => 0b1111 1111 if SW3 is not pushed
	//
	// Other techniques (Shifting and bitwise AND)
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 0 if SW3 is pushed
	// ((GPIO_PORTE_DATA_R >> 2) & 1) => 1 if SW3 is not pushed

    // TODO: Write code below -- Checks the leftmost bit, which corresponds to 6

    if(((GPIO_PORTE_DATA_R >> 5) & 1) == 0){
        return 6;
    }
    if(((GPIO_PORTE_DATA_R >> 4) & 1) == 0){
        return 5;
    }
    if(((GPIO_PORTE_DATA_R >> 3) & 1) == 0){
        return 4;
    }
    if(((GPIO_PORTE_DATA_R >> 2) & 1) == 0){
        return 3;
    }
    if(((GPIO_PORTE_DATA_R >> 1) & 1) == 0){
        return 2;
    }
    if(((GPIO_PORTE_DATA_R >> 0) & 1) == 0){
        return 1;
    }

    return 0; //EDIT ME
}


