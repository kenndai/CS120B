/*	Author: Kenny Dai	
 *  Partner(s) Name: 
 *	Lab Section: 024
 *	Assignment: Lab #14 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *  Follower
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#include "USART.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char data = 0x00;

void Tick_receive() {
	if (USART_HasReceived(1)) { 
		data = USART_Receive(1); //receive sent data
		PORTA = data; //display on PORTA
		USART_Flush(1); 
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
    /* Insert your solution below */
	
	data = 0; initUSART(1); USART_Flush(1);

	TimerSet(100);
	TimerOn();

    while (1) {
		while (!TimerFlag);
		Tick_receive();	
		TimerFlag = 0;
    }
    return 1;
}
