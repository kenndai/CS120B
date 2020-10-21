/*	Author: Kenny Dai kdai002@ucr.edu
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

    unsigned char numOnes;
    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char i;

    /* Insert your solution below */
    while (1) {
	tmpA = PINA; tmpB = PINB;
	numOnes = 0;
        for (i = 0; i < 8; i++) {
	    numOnes += ((tmpA >> i) & 0x01) + ((tmpB >> i) & 0x01);
	}
	PORTC = numOnes; //assign PORTC with the number of ones
    }
    return 1;
}
