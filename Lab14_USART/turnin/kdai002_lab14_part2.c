/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #14  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "USART.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

typedef struct task {
	int state;
	unsigned long period;
	unsigned long elapsedTime;
	int (*TickFct)(int);
} task;

task tasks[2];
const unsigned short tasksNum = 2;
const unsigned long tasksPeriodGCD = 10;

#include "timer.h"

enum leaderStates { leaderStart, leaderLoop };

unsigned char data = 0;
unsigned char transmitted = 0;

int leaderTick(int state) {
	switch (state) {
		case leaderStart: 
			PORTC = 0;
			state = (PINB & 0x01) ? leaderLoop : leaderStart; break;
		case leaderLoop:
			PORTC = 1;
			if (USART_IsSendReady(1)) { //USART1 sends
				PORTA = data; //display the same data on own PORTA
				USART_Send(data, 1); //send
				while (!USART_HasTransmitted(1)) { continue; }
				data = (data == 0x01) ? 0x00 : 0x01; 
			}
			state = (PINB & 0x01) ? leaderLoop : leaderStart;
			break;
		default: state = leaderStart; break;
	}
	return state;
}

enum followerStates { followerStart, followerLoop };

int followerTick(int state) {
	switch (state) {
		case followerStart: 
			PORTC = 1;
			state = (PINB & 0x01) ? followerStart : followerLoop; break;
		case followerLoop:
			PORTC = 0;
			if (USART_HasReceived(0)) //USART0 receives
				{ PORTA = transmitted = USART_Receive(0); USART_Flush(0); } //display then flush
			state = (PINB & 0x01) ? followerStart : followerLoop;
			break;
		default: state = followerStart; break;
	}
	return state;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	initUSART(0); initUSART(1);
	USART_Flush(0); USART_Flush(1);

	data = 0x01;

	unsigned char i = 0;
	tasks[i].state = leaderStart;
	tasks[i].period = 1000;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &leaderTick;
	++i;
	tasks[i].state = followerStart;
	tasks[i].period = 100;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &followerTick;

	TimerSet(tasksPeriodGCD);
	TimerOn();

    while (1) {}
    return 1;
}
