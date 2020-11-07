/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #6  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Video: 
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;

    /* Insert your solution below */
	TimerSet(1000); //1 sec 
	TimerOn();
	unsigned char tmpB = 0x00;
    while (1) {
		//User code (i.e. synchSM calls)
		tmpB = ~tmpB; // Toggle PORTB; Temporary, bad programming style
		PORTB = tmpB;
		while (!TimerFlag); //Wait 1 sec
		TimerFlag = 0;
    }
    return 1;
}
