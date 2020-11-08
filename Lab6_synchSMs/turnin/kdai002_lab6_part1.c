/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Video: https://www.youtube.com/watch?v=BQlbeB1mNq4
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Loop } state;

unsigned char led = 0;

void Tick() {
	switch(state) {
		case Start: led = 1; state = Loop; break;
		case Loop: 
			if (led == 4) led = 1;
			else led = led << 1;
			break;
	}	

	PORTB = led;

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;
	
	state = Start;
	TimerSet(1000); 
	TimerOn();
    while (1) {
		Tick();
		while (!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
