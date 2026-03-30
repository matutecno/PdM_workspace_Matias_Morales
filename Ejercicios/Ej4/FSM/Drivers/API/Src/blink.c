/*
 * blink.c
 *
 *  Created on: 30 mar 2026
 *      Author: Matías Morales
 */

#include "gpio.h"
#include "main.h"
#include "del.h"

static delay_t del;

void blink(int PER){
	static bool_t st = false;			//Estado del led
	if( ! delayRead(&del) ){
		delayWrite(&del, PER);
		st = !st;
	}
	if(st)
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

}
