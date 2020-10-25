/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #4  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, Increment, Decrement, Reset} state;

void Tick() {

	//transitions
	switch(state) {
		case Start: PORTC = 7; state = Wait; break;
		case Wait:
			if (PINA == 3) { PORTC = 0; state = Reset; } 
			else if (PINA == 1 && PINC != 9) { PORTC++; state = Increment; }
			else if (PINA == 2 && PINC != 0) { PORTC--; state = Decrement; }
			else state = Wait;
			break;
		case Increment:
			if (PINA == 3) { PORTC = 0; state = Reset; }
			//if 1 is held down remain in the same state
			else state = (PINA == 1) ? Increment : Wait;
			break;
		case Decrement: 
			if (PINA == 3) { PORTC = 0; state = Reset; }
			//if 2 is held down, remain in the same state
			else state = (PINA == 2) ? Decrement : Wait;
			break;
		case Reset: 
			//if buttons are held down remain in the same state
			state = (PINA == 3) ? Reset : Wait;
			break;
	}

/*
	switch(state) {
		case Start: break;
		case Wait: break;
		case Increment: PORTC++; break;
		case Decrement: PORTC--; break;
		case Reset: PORTC = 0; break;
	}
*/

}

int main() {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
		
	state = Start;
		
	while(1) {
		Tick();
    }
    return 0;
}

