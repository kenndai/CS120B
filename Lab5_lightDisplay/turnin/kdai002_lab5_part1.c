/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #5  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://www.youtube.com/watch?v=mVb7PnA6dWo
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF; //[3-0] for fuel levels
    DDRC = 0xFF; PORTC = 0x00; //[5-0] for fuel level lights, [6] for low fuel

    unsigned char tmpA = 0x00;
    unsigned char led = 0x00;

    /* Insert your solution below */
    while (1) {
	tmpA = ~PINA & 0x0F; //get only [3-0]
	led = 0x00;
	switch(tmpA) {
	    case 0: led = 0x40; break;
	    case 1:
	    case 2: led = 0x60; break;
	    case 3:
	    case 4: led = 0x70; break;
	    case 5:
	    case 6: led = 0x38; break;
	    case 7:
	    case 8:
	    case 9: led = 0x3C; break;
	    case 10:
	    case 11:
	    case 12: led = 0x3E; break;
	    case 13:
	    case 14:
	    case 15: led = 0x3F; break;
	}
	PORTC = led;
    }
    return 1;
}
