#include "del.h"

void delayInit( delay_t * delay, tick_t duration ){
	delay->running = true;
	delay->startTime = HAL_GetTick();
	delay->duration = duration;
};

bool_t delayRead( delay_t * delay ){
	uint32_t aux = HAL_GetTick() - delay->startTime;
	if(aux >= delay->duration)
		delay->running = false;
	return delay->running;
};


void delayWrite( delay_t * delay, tick_t duration ){
	if(!(delay->running)){
		delayInit(delay, duration);
	}
};
