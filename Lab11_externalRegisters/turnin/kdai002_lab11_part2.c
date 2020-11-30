/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #11 Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo video: https://youtu.be/ePlfXaMk8gU 
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum firstStates {fStart, LeftLoop, RightLoop} fState;
enum secondStates {sStart, OutLoop, InLoop} sState;
enum thirdStates {tStart, One, LeftShift, OneTwoEight, LeftShift2} tState;
enum monitorStates {mStart, decider, first, second, third, Off} mState;

unsigned char led;
unsigned char currentLight;
unsigned char A0, A1;
unsigned char firstHalf, secondHalf;
unsigned char otherLight;

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

void Tick_firstLights() {
	A0 = ~PINA & 0x01; //PINA == 1
    A1 = ~PINA & 0x02; //PINA == 2

	switch(fState) {
		case fStart: led = 1; fState = LeftLoop; break;
		case LeftLoop:
			if (led == 128) { led >>= 1; fState = RightLoop; }
			else led = led << 1;
			break;
		case RightLoop:
			if (led == 1) { led <<= 1; fState = LeftLoop; }
			else led = led >> 1;
			break;
	}
	transmit_data(led);
}

void Tick_secondLights() {
	//unsigned char firstHalf, secondHalf;

	switch(sState) {
		case sStart: led = 0; firstHalf = 8; secondHalf = 1; sState = OutLoop; break;
		case OutLoop:
			if (secondHalf == 8 && firstHalf == 1) { // if both halfs are outermost
				led = (secondHalf << 4) + firstHalf;		
				secondHalf >>= 1; firstHalf <<= 1;
				sState = InLoop;
			}
			else { // else keep going outwards
				led = (secondHalf << 4) + firstHalf;
				secondHalf <<= 1; firstHalf >>= 1;
				sState = OutLoop;
			}
			break;
		case InLoop:
			if (secondHalf == 1 && firstHalf == 8) { //if both halfs are innermost
				led = (secondHalf << 4) + firstHalf;
				secondHalf <<= 1; firstHalf >>= 1;
				sState = OutLoop;
			}
			else { //else keep going inwards
				led = (secondHalf << 4) + firstHalf;
				secondHalf >>= 1; firstHalf <<= 1; 
			}
			break;
	}
	transmit_data(led);
}

void Tick_thirdLights() {	

	switch(tState) {
		case tStart: otherLight = 1; tState = One; break;
		case One: led = 1; tState = LeftShift; break;
		case LeftShift: 
			if (otherLight == 128) otherLight = 1;
			otherLight <<= 1;
			led = otherLight;
			tState = OneTwoEight;
			break;
		case OneTwoEight: led = 128; tState = LeftShift2; break;
		case LeftShift2:
			if (otherLight == 128) otherLight = 1;
			otherLight <<= 1;
			led = otherLight;
			tState = One;
			break;
	}
	transmit_data(led);
}

void Tick_monitor() {
	A0 = ~PINA & 0x01; //PINA == 1
    A1 = ~PINA & 0x02; //PINA == 2
	
	switch(mState) {
		case mStart: currentLight = 0; mState = decider; break;
		case decider:
			if (A0 && A1) mState = Off;
			else if (currentLight == 0) mState = first;
			else if (currentLight == 1) mState = second; 
			else if (currentLight == 2) mState = third;
			else if (currentLight == 4) mState = Off;
			break;

		case first:
			Tick_firstLights();
			if (A0) { currentLight++; mState = decider; }
			else mState = first;	
			break;

		case second:
			Tick_secondLights();
			if (A0) { currentLight++; mState = decider; }
			else if (A1) { currentLight--; mState = decider; }
			else mState = second;
			break;

		case third:
			Tick_thirdLights();
			if (A1) { currentLight--; mState = decider; }
			else mState = third;
			break;
		case Off:
			transmit_data(0x00);
			currentLight = 4;
			mState = (A0 && A1) ? mStart : Off;
			break;
	}
	
//	transmit_data(led);

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    /* Insert your solution below */
/*
	unsigned long FL_elapsedTime = 0;
	unsigned long SL_elapsedTime = 0;
	unsigned long TL_elapsedTime = 0;
*/

	fState = fStart;
	sState = sStart;
	tState = tStart;
	mState = mStart;

	TimerSet(100);
	TimerOn();
    
	while (1) {
		Tick_monitor();
		while (!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
