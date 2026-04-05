/*
 * u_delay.c
 *
 *  Created on: 30 mar 2026
 *      Author: matute
 */

/*
 * SystemCoreClock = 72 000 000 (72 MHz). Dividiendo esto en 1 mega, tenemos que cada 72 ciclos es 1 uS.
 * DWT tiene un registro CYCCNT que se incrementa cada ciclo de reloj.
 *
 */

#include "u_delay.h"


void delayUsDriverInit(void){
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

void delayUsInit(delUs_t * del_us, uint32_t uS_dur){
	del_us->start = DWT->CYCCNT;
	del_us->running = false;
	del_us->duration = uS_dur;
};

bool delayUsRead(delUs_t * del_us){
	uint32_t cycles =  del_us->duration * (SystemCoreClock / 1000000U);			//Ej, 10 uS son 720 ciclos
	if ((DWT->CYCCNT - del_us->start) >= cycles)
		del_us->running = true;
	return del_us->running;
}


