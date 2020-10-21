/*	Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section: 24
 *	Assignment: Lab #2  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF; // Configure port A's 8 pins as inputs
	DDRC = 0xFF; PORTC = 0x00; // Configure port B's 8 pins as outputs, initialize to 0s
 	unsigned char cntavail = 0x00; // Temporary variable to hold the value of spaces available
	unsigned char tmpA = 0x00; // Temporary variable to hold the value of A
	while(1) {	
	    // 1) Read input
	    tmpA = PINA & 0x0F; //tmpA set to only the last four bits of PORTA
	    if (tmpA == 0x00)
		 cntavail = 0x04;
	    else if (tmpA == 0x01 || tmpA == 0x02 || tmpA == 0x04 || tmpA == 0x08)
		 cntavail = 0x03;
	    else if (tmpA == 0x03 || tmpA == 0x05 || tmpA == 0x06 || tmpA == 0x09 || tmpA == 0x0A || tmpA == 0x0C)
		 cntavail = 0x02;
	    else if (tmpA == 0x07 || tmpA == 0x0B || tmpA == 0x0D || tmpA == 0x0E)
		 cntavail = 0x01;
	    else
		cntavail = 0x80;	    
   	    // 3) Write output
	    PORTC = cntavail;    
   	}
    return 0;
}
