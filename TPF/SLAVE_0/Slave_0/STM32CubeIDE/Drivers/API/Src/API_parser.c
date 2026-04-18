/*
 * API_parser.c
 *
 *  Created on: 5 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *      SLAVE
 */

#include "API_parser.h"

#define DEL_UART 500000


static payload_t payload = {0};

typedef enum{
	MEASURE,
	MEASURING,
	UART,
	NRF,
	WAITING
}state_t;

static state_t state;
static delUs_t del;
static uint16_t med;

bool parserInit(UART_HandleTypeDef *huart, SPI_HandleTypeDef *hspi, TIM_HandleTypeDef *tim, uint8_t channel, GPIO_TypeDef * trig_port, uint16_t trig_pin){
	bool st = false;
	delayUsSet(&del, 500000);
	const uint8_t addr[5] = {0xEF, 0xEF, 0xEF, 0xEF, 0xEF};
	if(uartInit(huart) && jsnInit(tim, channel, trig_port, trig_pin) && nrfInit(hspi)){
		st = true;
		nrfSetModeTX(addr, NRF_CH, sizeof(payload_t));
	}
	else
		st = false;

	if(nrfReadReg(NRF_CONFIG) != 0x0E)
		NVIC_SystemReset();
	state = MEASURE;
	return st;
}

void updateParser(){

	switch (state){
	case MEASURE:{
		jsnTrigger();
		state = MEASURING;
	};break;
	case MEASURING: {
		jsnTrigger();
		if(jsnIsReady()){
			med = jsnGetDistance();
			state = UART;
		}
	}; break;
	case WAITING:{
		if(delayUsRead(&del))
			state = MEASURE;
	}; break;
	case UART:{
		uint8_t buf[32];
		snprintf((char * ) buf, sizeof(buf), "Medicion: %u cm \r\n", med);
		uartSendString(buf);
		delayUsSet(&del, DEL_UART);
		state = NRF;
		//if()

	}; break;
	case NRF:{
	      payload.nivel = med;
	      nrfTransmit((uint8_t*)&payload, sizeof(payload_t));
	      state = WAITING;
	}; break;
	}//sw
}






