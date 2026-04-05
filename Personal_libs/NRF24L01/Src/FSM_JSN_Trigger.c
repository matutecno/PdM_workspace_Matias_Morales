/*
 * FSM_JSN_Trigger.c
 *
 *  Created on: 30 mar 2026
 *      Author: Matías Morales
 */
#include "main.h"
#include "FSM_JSN_Trigger.h"
#include "u_delay.h"

#define T_H 10
#define T_L 100

delUs_t del10, del100;

static st_jsn_trig state;

void initTrigger(){
	state = STATE_H;
	delayUsInit(&del10, T_H);
}


void updateStateTrigger(){
	switch (state){
		case STATE_H:{
			HAL_GPIO_WritePin(JSN_TRIG_GPIO_Port, JSN_TRIG_Pin, GPIO_PIN_SET);
			if( delayUsRead(&del10) ){
				state = STATE_L;
				delayUsInit(&del100, T_L);
			}
		};break;
		case STATE_L:{
			HAL_GPIO_WritePin(JSN_TRIG_GPIO_Port, JSN_TRIG_Pin, GPIO_PIN_RESET);
			if( delayUsRead(&del100) ){
				state = STATE_H;
				delayUsInit(&del10, T_H);
			}
		}
	};//Switch
}
