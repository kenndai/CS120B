/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #7 Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Video Demo: https://youtu.be/2qudu7GY17Q
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital converstion
	// ADSC: setting this bit starts the first conversion
	// ADATE: setting this bit enables auto-triggering. Since we are 
	//		  in Free Running Mode, a new conversion will trigger
	//		  whenever the previous conversion completes
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
	unsigned short x, led;
	unsigned short MAX;
	ADC_init();
	
    while (1) {
		x = ADC; MAX = 0xF0;
		led = (x >= MAX/2) ? x : 0x00;
		PORTB = led;
    }
    return 1;
}
