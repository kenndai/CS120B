/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #14  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo video: https://youtu.be/6XPJc5CGnsQ
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

task tasks[1];
const unsigned short tasksNum = 1;
const unsigned long tasksPeriodGCD = 1;

#include "timer.h"

unsigned char data = 0;
unsigned short i, j;
const unsigned short LeaderPeriod = 500;
const unsigned short FollowerPeriod = 100;

enum States {Start, Leader, Follower };

/*
todo:

i not incrementing?

*/


int Tick(int state) {
	switch (state) {
		case Start:
			i = j = 0; data = 0x01;			
			state = (PINB & 0x01) ? Leader: Follower;
			break;
		case Leader:
			PORTC = 0x01;
			//PORTA = 1;
			if (i >= LeaderPeriod) { //Transmit every LeaderPeriod
				if (USART_IsSendReady(1)) { //check if ready to transmit, then send
					PORTA = data;
					USART_Send(data, 1); 
					while (!USART_HasTransmitted(1)) {};
					data = (data == 0x01) ? 0x00 : 0x01; //switch lights on or off
					i = 0; //reset i
				}
			}
			else ++i;

			if (PINB & 0x01) state = Leader;
			else { i = 0; j = 0; state = Follower; } //change state

			break;
		case Follower:
			PORTC = 0x00;

			if (j >= FollowerPeriod) { //Receive every FollowerPeriod
				if (USART_HasReceived(0)) { //check if U0 has received the data, then receive
					PORTA = USART_Receive(0); 
					USART_Flush(0); //empty
					j = 0; //reset j
				}
			}
			else ++j;
	
			if (PINB & 0x01) { i = 0; j = 0; state = Leader; } //change state
			else state = Follower;

			break;

		default: state = Start; break;
	}
	return state;
}

int main(void) {
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;

	initUSART(0); initUSART(1);
	USART_Flush(0); USART_Flush(1);

	unsigned char i = 0;
	tasks[i].state = Start;
	tasks[i].period = 1;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &Tick;
	++i;

	TimerSet(tasksPeriodGCD);
	TimerOn();

    while (1) {}
    return 1;
}
