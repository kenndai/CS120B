/*  Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *  Lab Section: 24
 *  Assignment: Lab #12 Exercise #4
 *  Exercise Description: [optional - include for your own benefit]
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *  Demo video:
 */

#include <avr/io.h>
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
const unsigned long tasksPeriodGCD = 1;

#include "timer.h"

unsigned char pattern, row; //shared variables between the synchSMs
unsigned char UpBut, DownBut, LeftBut, RightBut;

//maniuplate array elements when a button is pressed
unsigned char rectPatterns[] = { 0x3C, 0x24 };
unsigned char rectRows[] = { 0xF5, 0xFB };

//First SynchSM
enum MoveStates {moveStart, Wait, Up, Down, Left, Right} moveState;

int moveTick(int state) {

    UpBut = ~PINA & 0x01; //go up
    DownBut = ~PINA & 0x02; // go down
	LeftBut = ~PINA & 0x03; //go left;
	RightBut = ~PINA & 0x04; //go right

    switch (state) {
        case moveStart: state = Wait; break;
        case Wait: 
			if (UpBut) {
				state = Up;
			}
			else if (DownBut) {
				state = Down;
			}
            else if (LeftBut && (pattern != 0x80)) {
                state = Left;
            }
            else if (RightBut && (pattern != 0x01)) {
                state = Right;
            }
            else state = Wait;
            break;
		case Up: state = UpBut ? Up : Wait; break;
		case Down: state = DownBut ? Down : Wait; break;
        case Left: state = LeftBut ? Left : Wait; break;
        case Right: state = RightBut ? Right : Wait; break;
        default: state = moveStart; break;
    }

    PORTC = pattern;    // Pattern to display
    PORTD = row;        // Row(s) displaying pattern    
    return state;
}

//Second SynchSM
enum RectStates {rectStart, TopBot, Mid} rectState;

int rectTick(int state) {
    pattern = rectPatterns[0];
    row = rectRows[0];

    switch(state) {
        case rectStart: state = TopBot; break;
        case TopBot: pattern = rectPatterns[1]; row = rectRows[1]; state = Mid; break;
        case Mid: pattern = rectPatterns[0]; row = rectRows[0]; state = TopBot; break;
    }

    PORTC = pattern;
    PORTD = row;
    return state;
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
                                           
    unsigned char i = 0;

    tasks[i].state = moveStart;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &moveTick;
    ++i;
    tasks[i].state = rectStart;
    tasks[i].period = 1;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &rectTick;

    TimerSet(tasksPeriodGCD);
    TimerOn();

    while (1) {}
    return 0;
}

