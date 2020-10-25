/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #4  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Release1, Press1, Release2, Press2} state;

void Tick() {

	//transitions
    switch(state) {
		case Start: PORTB = 1; state = Release1; break;
		case Release1:
			if (PINA) {
				PORTB = 2;
				state = Press1;
			}
			else 
				state = Release1;
			break;
		case Press1:
			state = PINA ? Press1 : Release2;
			break;
		case Release2:
			if (PINA) {
				PORTB = 1;
				state = Press2;
			}
			break;
		case Press2:
			state = PINA ? Press2 : Release1;
			break;
	}
}
/*
	//action
	switch(state) {
		case Start: break;
		case Release1: PORTB = 1; break;
		case Press1: break
		case Release2: PORTB = 2; break;
		case Press2: break;
	}
*/

int main() {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
        DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
		
		state = Start;
		
		while(1) {
			Tick();
        }
    return 0;
}

