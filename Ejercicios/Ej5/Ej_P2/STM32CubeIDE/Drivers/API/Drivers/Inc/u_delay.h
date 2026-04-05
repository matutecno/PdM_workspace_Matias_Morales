/*
 * u_delay.h
 *
 *  Created on: 30 mar 2026
 *      Author: Matías Morales Gariglio
 *
 *      Delay no bloqueante en uS
 *
 *      IMPORTANTE: main.c debe iniciar el driver: delayUsDriverInit()
 */


#ifndef DRIVERS_API_INC_U_DELAY_H_
#define DRIVERS_API_INC_U_DELAY_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include <stdbool.h>

typedef struct{
	uint32_t start;
	uint32_t duration;
	bool running;
}delUs_t;

void delayUsInit(delUs_t * del_us, uint32_t uS_dur);
bool delayUsRead(delUs_t * del_us);
void delayUsDriverInit(void);
void delayUsSet(delUs_t * del_us, uint32_t uS_dur);

#endif /* DRIVERS_API_INC_U_DELAY_H_ */
