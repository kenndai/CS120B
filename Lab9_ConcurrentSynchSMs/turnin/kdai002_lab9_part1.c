/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #9  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 * Demo video: https://youtu.be/uuwTz-Ikr5g
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum ThreeStates {tStart, Loop} tState;
enum BlinkStates {bStart, On, Off} bState;
enum CombineStates {cStart, Combine} cState;

unsigned char threeLEDs;
unsigned char blinkingLEDs;
unsigned char combineLEDs;

void Tick_ThreeLEDsSM() {
	switch(tState) {
        case tStart: threeLEDs = 1; tState = Loop; break;
        case Loop: 
            if (threeLEDs == 4) threeLEDs = 1;
            else threeLEDs = threeLEDs << 1;
            break;
    }   	
	PORTB = threeLEDs;
}

void Tick_BlinkingLEDSM() {
	switch(bState) {
		case bStart: bState = On; break;
		case On: blinkingLEDs = 8; bState = Off; break;
		case Off: blinkingLEDs = 0; bState = On; break;
	}
}

void Tick_CombineLEDsSM() {
	switch(cState) {
		case cStart: cState = Combine; break;
		case Combine: combineLEDs = threeLEDs | blinkingLEDs; break;
	}
	PORTB = combineLEDs;
}

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;

	TimerSet(1000);
	TimerOn();
	
	tState = tStart;
	bState = bStart;
	cState = cStart;

	while(1) {
		Tick_ThreeLEDsSM();
		Tick_BlinkingLEDSM();
		Tick_CombineLEDsSM(); 
		while(!TimerFlag);
		TimerFlag = 0;
    }

    return 1;
}
