/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #14  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Demo video:
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
const unsigned long tasksPeriodGCD = 10;

#include "timer.h"

unsigned char data = 0;
unsigned char i;
unsigned short j;
const unsigned char FollowerPeriod = 10;
const unsigned short ReceivePeriod = 300;

enum States {Start, Follower };

int Tick(int state) {
	switch (state) {
		case Start:
			i = j = 0; data = 0x01;			
			state = Follower; //Start in Follower
			break;
		case Follower:
			PORTC = 0x00;
			
//			if (j < ReceivePeriod) { //If data has not been received for 3 seconds switch to leader
				++j;
				if (j % FollowerPeriod == 0) { //check every FollowerPeriod for data
					if (USART_HasReceived(0)) { //check if U0 has received the data
						PORTA = USART_Receive(0); 
						USART_Flush(0);
						j = 0; //only RESETS j if data HAS BEEN RECEIVED
					}	
				}
//			}

			state = Follower;
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
	tasks[i].period = 10;
	tasks[i].elapsedTime = 0;
	tasks[i].TickFct = &Tick;
	++i;

	TimerSet(tasksPeriodGCD);
	TimerOn();

    while (1) {}
    return 1;
}
