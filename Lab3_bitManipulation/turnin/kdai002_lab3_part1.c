/*	Author: Kenny Dai
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #3  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;

    unsigned char numOnes = 0x00;
    unsigned char pinA0 = 0x00;
    unsigned char pinB0 = 0x00;
    unsigned char i;

    /* Insert your solution below */
    while (1) {
        for (i = 0; i < 7; i++) {
	    pinA0 = PINA & 0x01; pinB0 = PINB & 0x01; //get first bits of PINA and PINB
	    if (pinA0 == 1 && pinB0 == 1) numOnes += 2; //if both bits == 1, add 2
	    else if (pinA0 == 1 || pinB0 == 1) numOnes += 1; //if only 1 bit == 1, add 1
	    PINA >>= 1; PINB >>= 1;
	}
	PORTC = numOnes; //assign PORTC with the number of ones
    }
    return 1;
}
