/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #8 Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo video: https://youtu.be/UMPCOfsFiE4
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

enum States {Start, Wait, Mute, SIncrement, SDecrement, MIncrement, MDecrement} state;

unsigned char A0, A1, A2;
unsigned char i;
double scale[] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

void Tick() {
	
	A0 = ~PINA & 0x01; // On/Off
	A1 = ~PINA & 0x02; // Scale up
	A2 = ~PINA & 0x04; // Scale down

	//even if off, i still can be incremented/decremented

	switch(state) {
		case Start: i = 0; state = Wait; break;
		case Wait:
			set_PWM(scale[i]);
			if (A0) { set_PWM(0); state = Mute; }
			else if (A1 && i < 7) { i++; state = SIncrement; }
			else if (A2 && i > 0) { i--; state = SDecrement; }
			else state = Wait;
			break;
		case Mute:
			if (A0) { set_PWM(scale[i]); state = Wait; }
			else if (A1 && i < 7) { i++; state = MIncrement; }	
			else if (A2 && i > 0) { i--; state = MDecrement; }
			else state = Mute;
			break;
		case SIncrement:
			state = (A1) ? SIncrement : Wait;
			break;
		case SDecrement:
			state = (A2) ? SDecrement : Wait;
			break;
		case MIncrement:
			state = (A1) ? MIncrement : Mute;
			break;
		case MDecrement:
			state = (A2) ? MDecrement : Mute;
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
