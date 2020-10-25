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

enum States {Start, Wait1, Press1, Wait2, Press2} state;

void Tick() {

	//transitions
    switch(state) {
    	case Start: PORTB = 0x01; state = Wait1; break;
		case Wait1: 
			if (PINA & 0x01) {
				PORTB = 0x02;
				state = Press1;
			}
			else state = Wait1;
			break;
		case Press1:
			if (PINA & 0x01) state = Press1; //holding button
			else state = Wait2;
		case Wait2:
			if (PINA & 0x01) {
				PORTB = 0x01;
				state = Press2;
			}
			else {
				state = Wait2;
			}
		case Press2:
			if (PINA & 0x01) state = Press2; //holding button
			else state = Wait1;
	} 


int main(void) {
    /* Insert DDR and PORT initializations */
        DDRA = 0x00; PORTA = 0x00; // Configure port A's 8 pins as inputs
        DDRB = 0xFF; PORTB = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
		
		state = Start;
		
		while(1) {
			Tick();
        }
    return 0;
}

