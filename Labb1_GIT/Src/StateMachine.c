/*
 * StateMachine.c
 *
 *  Created on: 31 okt. 2018
 *      Author: viktor
 */
#include "StateMachine.h"
#include "stm32f4xx_hal.h"

state_e state=IDLE_STATE;
event_e event = NOT_PRESSED_EVENT;
int digit=0;
uint8_t  led[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
Queue q;
int timeout=100;


void printDigit(int digit){
	GPIOC->ODR=led[digit];
}

void stateMachine(){
	event= ReadQueue(&q);

	switch (state) {
		case IDLE_STATE:
			printDigit(digit);
			if (event == PRESSED_BUTTON_EVENT){
				state = DEBOUNCE_STATE;
				timeout = 1000;
			}
			break;
		case DEBOUNCE_STATE:
			//event=ReadQueue(&q);
			if (event==DEBOUNCE_EVENT){
				timeout-=1;
				//WriteQueue(&q, DEBOUNCE_EVENT);
			}
			timeout_Callback();
			if(timeout == 0){
				WriteQueue(&q, COUNT_EVENT);
				state=BUTTON_PRESSED_STATE;
			}
			break;
		case BUTTON_PRESSED_STATE:
			if (event==COUNT_EVENT){
				digit+=1;
				if (digit>=10){
					digit=0;
				}
				printDigit(digit);
				state = IDLE_STATE;
				event = NOT_PRESSED_EVENT;
			}

			break;
		default:
			state = IDLE_STATE;
			//event = NOT_PRESSED_EVENT;
			break;
	}


}

void Init_Queue(Queue *Q){
	Q->readIx=0;
	Q->writeIx=0;
	Q->numberElem=0;
}

void WriteQueue(Queue *Q, event_e e){
	if (Q->numberElem >= 20){
		return;
	}
	__disable_irq();
	Q->buffer[Q->writeIx]=e;
	Q->writeIx=(Q->writeIx+1)%20;
	Q->numberElem++;
	__enable_irq();
}

event_e ReadQueue(Queue *Q){
	if(Q->numberElem==0){
		return NOT_PRESSED_EVENT;
	}
	event_e e;
	__disable_irq();
	e=Q->buffer[Q->readIx];
	Q->readIx=(Q->readIx+1)%20;
	Q->numberElem--;
	__enable_irq();
	return e;
}

void HAL_GPIO_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == GPIO_PIN_13){
		WriteQueue(&q, PRESSED_BUTTON_EVENT);
	}
}

void timeout_Callback(){
	WriteQueue(&q, DEBOUNCE_EVENT);
}

/*
 * intrupt(){
 * disable_irq();
 * event = PRESSED_BUTTON_EVENT;
 * enable_irq();
 * }
 *
 */

