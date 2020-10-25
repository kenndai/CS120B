/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #4  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Lock, PoundPress, PoundRelease, YPress, YRelease, Unlock} state;

void Tick() {

	//transitions
	switch(state) {
		case Start: PORTB = 0; state = Lock; break; //door closed initially
		case Lock: state = (PINA == 4) ? PoundPress : Lock; break; //must click pound to proceed
		case PoundPress:
			if (PINA == 4) state = PoundPress; //if # is held down
			else if (PINA == 0) state = PoundRelease; //# button released
			else state = Lock; //wrong input
			break;
		case PoundRelease:
			if (PINA == 2) state = YPress;
			else if (PINA == 0) state = PoundRelease; //wait for input
			else state = Lock; //wrong input
			break;
		case YPress:
			if (PINA == 2) state = YPress; //if Y is held down
			else if (PINA == 0) state = YRelease; //Y button released
			else state = Lock; //wrong input		
			break;
		case YRelease:
			PORTB = 1; state = Unlock; break;
		case Unlock:
			if (PINA == 0x80) {
				PORTB = 0;
				state = Lock;
			}
			else state = Unlock;
			break;
	}

}

int main() {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
	DDRB = 0xFF; PORTB = 0x00; // if open PB = 1, if closed PB = 0
		
	state = Start;
		
	while(1) {
		Tick();
    }
    return 0;
}

