/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #14  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *  Leader
 *	I acknowledge all content contained herein, excluding datalate or example
 *	code, is my own original work.
 *  Demo video: https://youtu.be/qeQENGv_d28
 */

#include <avr/io.h>
#include "USART.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char data = 0x00;

void Tick_transmit(){
	if (USART_IsSendReady(1)) {
		PORTA = data;
		USART_Send(data, 1);
		while(!USART_HasTransmitted(1)){
			continue;
	   	}
		data = (data == 0x01) ? 0x00 : 0x01;	
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;

	initUSART(1);
	
	TimerSet(1000);
	TimerOn();

    /* Insert your solution below */
    while (1) {
		while(!TimerFlag);
		TimerFlag = 0;
		Tick_transmit();
    }
    return 1;
}
