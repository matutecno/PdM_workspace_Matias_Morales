/*
 * led_blink.c
 *
 *  Created on: 5 abr 2026
 *      Author: Matías Morales Gariglio
 */

#include "led_blink.h"

static uint32_t _per = 0;

void initLed(led_blink_t * led, uint32_t periodo){
	led->led_stat = false;
	_per = periodo;
	delayUsInit(&led->del, _per);
	led->run = false;
	led->flame = false;
}

void updateStateLed(led_blink_t * led){
	if(led->flame)
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	else{
		if( led->run ){
			if( delayUsRead(&led->del) ){
				led->led_stat = !led->led_stat;
				delayUsSet(&led->del, _per);
			};

			if( led->led_stat )
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

		}
		else
			HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
	}//else

};
