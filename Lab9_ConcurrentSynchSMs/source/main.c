/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #9  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I aciknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Video Demo: https://youtu.be/WbaDXKx9cq0
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum ThreeStates {tStart, Loop} tState;
enum BlinkStates {bStart, On, Off} bState;
enum SoundStates {sStart, sLoop} sState;

unsigned char threeLEDs;
unsigned char blinkingLEDs;
unsigned char combineLEDs;
unsigned char switchSlide;

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

void Tick_SoundSM() {
	switchSlide = ~PINA & 0x04;

	switch(sState) {
		case sStart: sState = sLoop; break;
		case sLoop: 
			if (switchSlide) { combineLEDs = 0x10 | threeLEDs | blinkingLEDs; }
			else combineLEDs = threeLEDs | blinkingLEDs;
			break;
	}
	PORTB = combineLEDs;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	
	unsigned long TL_elapsedTime = 0;
	unsigned long BL_elapsedTime = 0;
	unsigned long S_elapsedTime = 0;
	const unsigned long timerPeriod = 1;

	TimerSet(timerPeriod);
	TimerOn();
	
	tState = tStart;
	bState = bStart;
	sState = sStart;

	while(1) {
		if (TL_elapsedTime >= 300) {
			Tick_ThreeLEDsSM();
			TL_elapsedTime = 0;
		}
        if (BL_elapsedTime >= 1000) {
            Tick_BlinkingLEDSM();
            BL_elapsedTime = 0;
        } 
		if (S_elapsedTime >= 2) {
			Tick_SoundSM();
			S_elapsedTime = 0;
		}

		while(!TimerFlag);
		TimerFlag = 0;

		TL_elapsedTime += timerPeriod;
		BL_elapsedTime += timerPeriod;		
		S_elapsedTime += timerPeriod;
    }

    return 1;
}
