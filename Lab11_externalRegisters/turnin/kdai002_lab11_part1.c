/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #11 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo video: https://youtu.be/18VKAMG6ZsE
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum States {Start, Wait, Increment, Decrement, Reset} state;
unsigned char led;
unsigned char A0, A1;

void transmit_data(unsigned char data) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
   	 PORTC = 0x08;
   	 // set SER = next bit of data to be sent.
   	 PORTC |= ((data >> i) & 0x01);
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 PORTC |= 0x02;  
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    PORTC |= 0x04;
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
}

void Tick() {

	A0 = ~PINA & 0x01; //PINA == 1
    A1 = ~PINA & 0x02; //PINA == 2

	switch(state) {
		case Start: led = 7; state = Wait; break;
		case Wait:
			if (A0 && A1) { led = 0; state = Reset; } 
			else if (A0 && led != 0xFF) { led++; state = Increment; }
			else if (A1 && led != 0) { led--; state = Decrement; }
			else state = Wait;
			break;
		case Increment:
			if (A0 && A1) { led = 0; state = Reset; }
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
	
	transmit_data(led);
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
	
	state = Start;

    while (1) {
		Tick();
    }
    return 1;
}
