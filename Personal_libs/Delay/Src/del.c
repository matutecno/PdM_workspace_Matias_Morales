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

#ifdef HAL_TIM_MODULE_ENABLED
void delayUs( TIM_HandleTypeDef *htim, uint16_t us ){
	htim->Instance->CNT = 0;
	HAL_TIM_Base_Start(htim);
	while(htim->Instance->CNT < us);
	HAL_TIM_Base_Stop(htim);
}
#endif
