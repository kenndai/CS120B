/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #5  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Video:
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, Increment, Decrement, Reset} state;

unsigned char A0, A1;
unsigned char led;

void Tick() {

    A0 = ~PINA & 0x01; //PINA == 1
    A1 = ~PINA & 0x02; //PINA == 2

	//transitions
	switch(state) {
		case Start: led = 7; state = Wait; break;
		case Wait:
			if (A0 && A1) { led = 0; state = Reset; } 
			else if (A0 && led != 9) { led++; state = Increment; }
			else if (A1 && led != 0) { led--; state = Decrement; }
			else state = Wait;
			break;
		case Increment:
			if (A0 && !A1) { led = 0; state = Reset; }
			//if 1 is held down remain in the same state
			else state = (A0) ? Increment : Wait;
			break;
		case Decrement: 
			if (A0 && A1) { led = 0; state = Reset; }
			//if 2 is held down, remain in the same state
			else state = (A1) ? Decrement : Wait;
			break;
		case Reset: 
			//if buttons are held down remain in the same state
			state = (A0 && A1) ? Reset : Wait;
			break;
	}

	PORTC = led;

}

int main() {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	led = 0;
	state = Start;
		
	while(1) {
		Tick();
    }
    return 0;
}

