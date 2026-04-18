/*
 * jsnsr04t.c
 *
 *  Created on: 5 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *      Importante: Agregar al main
 */

#include "jsnsr04t.h"

#define MAX_V 200

typedef struct{
	TIM_HandleTypeDef * 	tim;
	uint32_t 				capture_h, capture_l;
	uint16_t 				dist;
	uint8_t 				channel;
	volatile bool 			ready;		//Debe ser volatile para que sea modificada por el callback
	bool 					edge;
	GPIO_TypeDef *			trig_port;
	uint16_t      			trig_pin;
	delUs_t       			trig_delay;
	bool          			trig_st;
}jsn_t;

static jsn_t jsn;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef * htim){
	if(htim->Instance == TIM2){
		if(jsn.edge == false){			//Flanco de subida
			jsn.capture_h = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);	//Capturaremos flanco descendente
			jsn.edge = true;
		}else{
			jsn.capture_l = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
			jsn.dist = (jsn.capture_l - jsn.capture_h) / 59;										//Explicación al fondo
			if(jsn.dist > MAX_V)
				jsn.dist = 0;
			else
				jsn.ready = true;
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);	//Capturaremos flanco ascendente
			jsn.edge = false;
		}
	}
};

bool jsnInit(TIM_HandleTypeDef *tim, uint8_t channel, GPIO_TypeDef * trig_port, uint16_t trig_pin){
	jsn.tim = tim;
	jsn.channel = channel;
	jsn.trig_port = trig_port;
	jsn.trig_pin = trig_pin;
	jsn.trig_st = false;
	delayUsInit(&jsn.trig_delay, 0);
	HAL_TIM_IC_Start_IT(tim, channel);		//arranca el input capture con interrupciones
	return true;
};

uint16_t jsnGetDistance(void){
	jsn.ready = false;
	return jsn.dist;
}

bool jsnIsReady(){
	return jsn.ready;
};

void jsnTrigger(){			//Espera 100 ms, sube pin, espera 10 uS, baja pin
	if(!jsn.trig_st){
		if(delayUsRead(&jsn.trig_delay)){
			HAL_GPIO_WritePin(jsn.trig_port, jsn.trig_pin, GPIO_PIN_SET);
			delayUsSet(&jsn.trig_delay, 10);
			jsn.trig_st = true;
		}
	}else{
		if(delayUsRead(&jsn.trig_delay)){
			HAL_GPIO_WritePin(jsn.trig_port, jsn.trig_pin, GPIO_PIN_RESET);
			delayUsSet(&jsn.trig_delay, 100000);
			jsn.trig_st = false;
		}
	}
}



/*
 *
 * Lógica: La velocidad del sonido es 0.034cm/uS.
 * La inversa seria que cada cm tarda 29.4 uS. Como el pulso va y vuelve
 * cada cm de distancia real equivale a 58.8 uS
 */
