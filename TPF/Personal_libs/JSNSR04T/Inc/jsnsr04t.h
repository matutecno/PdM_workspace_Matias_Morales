/*
 * jsnsr04t.h
 *
 *  Created on: 5 abr 2026
 *      Author: matute
 */

#ifndef DRIVERS_PERSONAL_LIBS_JSNSR04T_INC_JSNSR04T_H_
#define DRIVERS_PERSONAL_LIBS_JSNSR04T_INC_JSNSR04T_H_

#include <stdbool.h>
#include "u_delay.h"

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

bool jsnInit(TIM_HandleTypeDef *tim, uint8_t channel, GPIO_TypeDef * trig_port, uint16_t trig_pin);				//Recibe el handler del time y arranca capture
uint16_t jsnGetDistance(void);		//Calcula la distancia
bool jsnIsReady(void);
void jsnTrigger(void);

#endif /* DRIVERS_PERSONAL_LIBS_JSNSR04T_INC_JSNSR04T_H_ */
