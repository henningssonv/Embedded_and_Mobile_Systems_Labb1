/*
 * StateMachine.h
 *
 *  Created on: 31 okt. 2018
 *      Author: viktor
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_



#endif /* STATEMACHINE_H_ */

typedef enum {
	IDLE_STATE,
	DEBOUNCE_STATE,
	BUTTON_PRESSED_STATE,
	WAIT_STATE,
	LIGHT_RIGHT_LEDBAR_STATE
} state_e;

typedef enum {
	PRESSED_BUTTON_EVENT,
	DEBOUNCE_EVENT,
	COUNT_EVENT,
	NOT_PRESSED_EVENT,
	NO_TICK_EVENT,
	TICK_EVENT
} event_e;

typedef struct{
	event_e buffer[20];
	int readIx;
	int writeIx;
	int numberElem;
}Queue;

extern Queue q;
extern Queue q_tick;

void Init_Queue(Queue *Q);
void WriteQueue(Queue *Q, event_e e);
event_e ReadQueue(Queue *Q);
void stateMachine_7seg();
void stateMachine_ledbar();
void timeout_Callback();
void tick_Callback();
void printDigit(int digit);
void printDigit_ledbar(int bar);

