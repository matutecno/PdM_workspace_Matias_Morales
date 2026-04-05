/*
 * led_blink.h
 *
 *  Created on: 5 abr 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef DRIVERS_API_DRIVERS_INC_LED_BLINK_H_
#define DRIVERS_API_DRIVERS_INC_LED_BLINK_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include "u_delay.h"
#include "main.h"
#include "led_blink.h"

typedef struct{
	delUs_t del;
	bool led_stat;
	bool run;		//run del blink
	bool flame;		//flame = 1, led encendido ctemente

}led_blink_t;


void initLed(led_blink_t * led, uint32_t periodo);
void updateStateLed(led_blink_t * led);


#endif /* DRIVERS_API_DRIVERS_INC_LED_BLINK_H_ */
