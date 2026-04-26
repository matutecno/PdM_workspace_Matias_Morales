/*
 * yfs201.h
 *
 *  Created on: 25 abr 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_H_
#define DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include "u_delay.h"
#include <stdbool.h>

void yfsInit(void);
void yfsCount(void);
uint16_t yfsGetCount(void);
void yfsReset(void);
uint16_t calcFlow (void);
bool yfsIsReady(void);
uint16_t yfsCalcFlow(void);
#endif /* DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_H_ */
