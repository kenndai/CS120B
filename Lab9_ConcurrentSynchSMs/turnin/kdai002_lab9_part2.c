/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #9  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Video Demo: https://youtu.be/BPqJHneN-tI
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
	
	unsigned long TL_elapsedTime = 0;
	unsigned long BL_elapsedTime = 0;
	const unsigned long timerPeriod = 100;

	TimerSet(100);
	TimerOn();
	
	tState = tStart;
	bState = bStart;
	cState = cStart;

	while(1) {
		if (TL_elapsedTime >= 300) {
			Tick_ThreeLEDsSM();
			TL_elapsedTime = 0;
		}
        if (BL_elapsedTime >= 1000) {
            Tick_BlinkingLEDSM();
            BL_elapsedTime = 0;
        } 
		
		Tick_CombineLEDsSM();

		while(!TimerFlag);
		TimerFlag = 0;

		TL_elapsedTime += timerPeriod;
		BL_elapsedTime += timerPeriod;		
    }

    return 1;
}
