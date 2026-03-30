/*
 * fsm.c
 *
 *  Created on: 26 mar 2026
 *      Author: Matías Morales
 *
 *
 *      Estados:
 *      E0 -> button_up
 *      E1 -> button_falling
 *      E2 -> button_down
 *      E3 -> button_rissing
 *
 *      Cada t_nm es la transición de un estado a otro
 *
 *      La estrategia de resolución de este ejercicio fue definir estados y transiciones
 */


#include <API_debounce.h>
#include "main.h"
#include "gpio.h"

#define HOLDING_TIME 40
#define TIME_AFTER_PUSH 200

bool_t readPush(){		//Función para leer pulsador
	if( ! HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) )
		return true;
	else
		return false;
};

delay_t del40;

static debounceState_t state;

void debugPush(){		//Función simple para ver si funciona led y pulsador
	if( readPush() )
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
};



bool_t t01(){			//Condición de transición estado 0 a estado 1
	return readPush();
}

bool_t t10(){			//Condición de transición estado 1 a estado 0

	if( ! readPush() && delayRead(&del40) )
		return true;
	else
		return false;

}

bool_t t12(){			//Condición de transición estado 1 a estado 2
	if( readPush() && ! delayRead(&del40) )
		return true;
	else
		return false;
};

bool_t t23(){			//Condición de transición estado 2 a estado 3
	if( ! readPush() )
		return true;
	else
		return false;
};

bool_t t32(){			//Condición de transición estado 3 a estado 2
	if( readPush() && delayRead(&del40) )
		return true;
	else
		return false;
};

bool_t t30(){			// Condición de transición de estado 3 a estado 0
	if( ! readPush() && ! delayRead(&del40) )
		return true;
	else
		return false;
};

void debounceFSM_init(){
	state = BUTTON_UP;
	//delayInit(&del40, HOLDING_TIME);
};

void debounceFSM_update(){
	switch(state){
		case BUTTON_UP:{
			//acciones
			if( t01() ){
				state = BUTTON_FALLING;
				delayWrite(&del40, HOLDING_TIME);
			};
		};break;//BUTTOM_UP
		case BUTTON_FALLING:{
			if( t12() ){
				state = BUTTON_DOWN;
			}else{
				if( t10() )
					state = BUTTON_UP;
			};
		};break; 	//button_falling

		case BUTTON_DOWN:{
			if( t23() ){
				delayWrite(&del40, HOLDING_TIME);
				state = BUTTON_RAISING;
			};
		};break;	//button_down

		case BUTTON_RAISING:{
			if( t30() ){
				state = BUTTON_UP;
			}else{
				if( t32() )
					state = BUTTON_DOWN;
			};
		};break;
	};//Switch
};

bool_t retStat(){
	if( state == BUTTON_DOWN ){
		return true;
	}
	else
		return false;
};
