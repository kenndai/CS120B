/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #8 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo video: https://youtu.be/yhVQgV_FDZg
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// 0.954 hz is lowest freq possible with this function/
// based on settings in PWN_on()
// Passing in 0 as the frequency will stop the speaker from generating sound
void set_PWM(double frequency) {
	static double current_frequency; //Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) {TCCR3B &= 0x08;} //stops timer/counter
		else { TCCR3B |= 0x03; } //resumes/continues timer/counter

		//prevents OCR3A from overflowing, using prescaler 64
 		//0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }

		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		//set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
		// COM3A0: Toggle PB3 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
		//WGM32: When counter (TCNT3) matches OCR3A, reset counter
		//CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States {Start, Wait/*, ButtonC, ButtonD, ButtonE*/} state;

unsigned char A0, A1, A2;

void Tick() {

	A0 = ~PINA & 0x01; //C
	A1 = ~PINA & 0x02; //D
	A2 = ~PINA & 0x04; //E

	switch(state) {
		case Start: set_PWM(0); state = Wait; break;
		case Wait:	
			if ((A0 && A1 && A2) || (A0 && A1) || (A1 && A2) || (A0 && A2)) set_PWM(0);
			else if (A0) set_PWM(261.63);
			else if (A1) set_PWM(293.66);
			else if (A2) set_PWM(329.63);
			else set_PWM(0);
			state = Wait;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
	state = Start;
    while (1) {
		Tick();
    }
    return 1;
}
