/*
 * u_delay.h
 *
 *  Created on: 30 mar 2026
 *      Author: Matías Morales Gariglio
 *      Delay no bloqueante en uS
 */


#ifndef DRIVERS_API_INC_U_DELAY_H_
#define DRIVERS_API_INC_U_DELAY_H_

#include "stm32f1xx_hal.h"
#include <stdbool.h>

typedef struct{
	uint32_t start;
	uint32_t duration;
	bool running;
}delUs_t;

void delayUsInit(delUs_t * del_us, uint32_t uS_dur);
bool delayUsRead(delUs_t * del_us);
void delayUsDriverInit(void);

#endif /* DRIVERS_API_INC_U_DELAY_H_ */
