/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #13  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo video: https://youtu.be/7Dsy_aZU-3o
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void A2D_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[2];
const unsigned short tasksNum = 2;
const unsigned long tasksPeriodGCD = 100;


#include "timer.h"

unsigned short input;
const unsigned short NEUTRAL = 540; 
unsigned char pattern, row;
//manipulate array elements when a button is pressed
unsigned char Patterns[] = { 0x10 };
unsigned char Rows[] = { 0xFE };
//First SynchSM
enum moveStates {moveStart, Wait};

int moveTick(int state) {

	input = ADC;

	switch (state) {
		case moveStart: state = Wait; break;
		case Wait:
			if (input < NEUTRAL) {
				if (pattern == 0x80) Patterns[0] = 0x01;
				else Patterns[0] <<= 1;	
			}
			else if (input > NEUTRAL) {
				if (pattern == 0x01) Patterns[0] = 0x80;
				else Patterns[0] >>= 1; 
			}
			state = Wait;
			break;
		default: state = moveStart; break;
	}
	return state;
}

//Second SynchSM
enum displayStates {displayStart, displayLoop};

int displayTick(int state) {
	switch(state) {
		case displayStart: state = displayLoop; break;
		case displayLoop: pattern = Patterns[0]; row = Rows[0]; state = displayLoop; break;
		default: state = displayStart; break;
	}
	PORTC = pattern;
	PORTD = row;
	return state;
}

int main(void) {
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;	

	A2D_init();

	unsigned char i = 0;
	tasks[i].state = moveStart;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &moveTick;
	i++;
	tasks[i].state = displayStart;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &displayTick;

	TimerSet(tasksPeriodGCD);
	TimerOn();

    while (1) {}
    return 1;
}
