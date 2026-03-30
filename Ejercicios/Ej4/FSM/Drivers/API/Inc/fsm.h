/*
 * fsm.h
 *
 *  Created on: 26 mar 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef API_INC_FSM_H_
#define API_INC_FSM_H_

#include "del.h"


void debounceFSM_init();		// debe cargar el estado inicial
void debounceFSM_update();		// debe leer las entradas, resolver la lógica de

// transición de estados y actualizar las salidas
void buttonPressed();			// debe encender el LED
void buttonReleased();			// debe apagar el LED
bool_t readPush();
void debugPush();

//El tiempo de anti-rebote debe ser de 40 ms con un retardo no bloqueante como los implementados en la práctica 3.

//La función debounceFSM_update() debe llamarse periódicamente.

typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
}debounceState_t;


#endif /* API_INC_FSM_H_ */
