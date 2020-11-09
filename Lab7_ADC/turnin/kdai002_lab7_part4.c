/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #7 Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Video Demo: https://youtu.be/ypb8qC3Ds6w
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

    /* Insert your solution below */
	unsigned short x, led;
	unsigned short MAX; //240
	unsigned char threshold;
	ADC_init();
	
    while (1) {
		x = ADC; MAX = 0xF0;
		threshold = MAX / 8; //30

		if (x < threshold) led = 0;
		else if (x < threshold * 2) led = 1;
		else if (x < threshold * 3) led = 3;
		else if (x < threshold * 4) led = 7;
        else if (x < threshold * 5) led = 15;
        else if (x < threshold * 6) led = 31;
        else if (x < threshold * 7) led = 63;
        else if (x < threshold * 8) led = 127;
		else led = 255;
		
		PORTB = led;
    }
    return 1;
}
