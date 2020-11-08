/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #6  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Video: https://youtu.be/T0sOmmdorJw
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, LeftLoop, RightLoop, Press, Restart} state;

unsigned char A0;
unsigned char led;

void Tick() {
	A0 = ~PINA & 0x01;

	switch(state) {
		case Start: led = 1; state = LeftLoop; break;
		case LeftLoop: 
			if (A0) state = Press; //if A0 is pressed transition to Press state
			else if (led == 4) { led >>= 1; state = RightLoop; }
			else led = led << 1;
			break;
		case RightLoop:
			if (A0) state = Press;
			else if (led == 1) { led <<= 1; state = LeftLoop; }
			else led = led >> 1;			
			break;
		case Press:
			state = A0 ? Press : Restart;
			break;
		case Restart:
			if (A0) state = Start;
			else state = Restart;
			break;
	}	

	PORTB = led;

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	
	state = Start;
	TimerSet(300); 
	TimerOn();
    while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
