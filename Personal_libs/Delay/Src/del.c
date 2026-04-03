#include "del.h"

void delayInit( delay_t * delay, tick_t duration ){
	delay->duration = duration;
	delay->running = false;
	delay->startTime = HAL_GetTick();
};

bool_t delayRead( delay_t * delay ){

	uint32_t aux = HAL_GetTick() - delay->startTime;
	if(aux >= delay->duration)
		delay->running = true;

	return delay->running;
};


void delayWrite( delay_t * delay, tick_t duration ){
	delay->duration = duration;
};


