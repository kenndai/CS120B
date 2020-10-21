/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #3  Exercise #4
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
    DDRA = 0x00; PORTA = 0xFF; //inputs
    DDRB = 0xFF; PORTB = 0x00; //outputs B[3-0] gets the upper half of A
    DDRC = 0xFF; PORTC = 0x00; //outputs C[7-4] gets the lower half of A

    unsigned char tmpA, tmpB, tmpC = 0x00;

    /* Insert your solution below */
    while (1) {
	tmpC = PINA & 0x0F;
	tmpB = PINA >> 4;
	PORTB = tmpB;
	PORTC = tmpC << 4;
    }
    return 1;
}
