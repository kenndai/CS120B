/*  Author: Kenny Dai kdai002@ucr.edu
 *  Partner(s) Name: 
 *  Lab Section: 24
 *  Assignment: Lab #12 Exercise #3
 *  Exercise Description: [optional - include for your own benefit]
 *
 *  I acknowledge all content contained herein, excluding template or example
 *  code, is my own original work.
 *  Demo video: https://youtu.be/nIHdD-xVr7Y 
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

unsigned char A0, A1; 

enum MoveStates {Start, Wait, Left, Right} moveState;

int moveTick(int state) {
    static unsigned char pattern = 0x3C;    // LED pattern - 0: LED off; 1: LED on
    static unsigned char row = 0xF5;    // Row(s) displaying pattern. 
                            // 0: display pattern on row
                            // 1: do NOT display pattern on row
    A0 = ~PINA & 0x01; //go up
    A1 = ~PINA & 0x02; // go down

    switch (state) {
        case Start: state = Wait; break;
        case Wait: 
            if (A0 && (pattern != 0x80)) {
                pattern <<= 1;
                state = Left;
            }
            else if (A1 && (pattern != 0x01)) {
                pattern >>= 1;
                state = Right;
            }
            else state = Wait;
            break;
        case Left:
            state = A0 ? Left : Wait;
            break;
        case Right:
            state = A1 ? Right : Wait;
            break;
        default: state = Start; break;
    }

    PORTC = pattern;    // Pattern to display
    PORTD = row;        // Row(s) displaying pattern    
    return state;
}

enum RectStates {rectStart, TopBot, Mid} rectState;

unsigned char rectPatterns[] = { 0x3C, 0x24 };
unsigned char rectRows[] = { 0xF5, 0xFB };

int rectTick(int state) {
    unsigned char pattern = rectPatterns[0];
    unsigned char row = rectPatterns[0];

    switch(state) {
        case Start: state = TopBot; break;
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

    tasks[i].state = Start;
    tasks[i].period = 100;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &moveTick;
    ++i;
    tasks[i].state = Start;
    tasks[i].period = 1;
    tasks[i].elapsedTime = 0;
    tasks[i].TickFct = &rectTick;

    TimerSet(tasksPeriodGCD);
    TimerOn();

    while (1) {}
    return 0;
}

