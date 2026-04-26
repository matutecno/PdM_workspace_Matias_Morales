/*
 * yfs201.c
 *
 *  Created on: 25 abr 2026
 *      Author: Matías Morales Gariglio
 */

#include "yfs201.h"
#include "yfs201_ports.h"

#define SAMP 1000000
static delUs_t sample;

static volatile uint16_t _pulse_cont, _flow;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){		//Guarda! Ya definida
	if(GPIO_Pin == FLOW_SENSOR_Pin)
		yfsCount();
};

void yfsInit(void){
	delayUsInit(&sample, 1);
	yfsReset();
};

void yfsCount(){
	_pulse_cont++;
};


uint16_t yfsGetCount(void){
	return _pulse_cont;
};


void yfsReset(void){
	_pulse_cont = 0;
	_flow = 0;
};

bool yfsIsReady(void){
	if(delayUsRead(&sample))
		return true;
	else
		return false;
}

uint16_t yfsCalcFlow(void){
	if(delayUsRead(&sample)){
		_flow = _pulse_cont;
		delayUsSet(&sample, SAMP);
		return _flow;
	}else
		return 0;

};
