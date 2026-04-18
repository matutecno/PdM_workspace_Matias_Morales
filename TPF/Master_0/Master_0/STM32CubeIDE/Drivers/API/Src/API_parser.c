/*
 * API_parser.c
 *
 *  Created on: 11 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *      MASTER
 */

#include "API_parser.h"

#define READ_UPD 10000000		//Si en 10 segundos no leo nada, apago todo el diplay

typedef enum{
	RECEIVE,
	WAIT,
	UART,
	PRINT_LEV
}states_fsm_t;

static states_fsm_t state;
static const uint8_t addr[5] = {0xEF, 0xEF, 0xEF, 0xEF, 0xEF};
static uint8_t buff[32];
static delUs_t del, del_read;

static payload_t payload = {0};

bool initParser(UART_HandleTypeDef * huart, SPI_HandleTypeDef * hspi){
    delayUsDriverInit();
    delayUsInit(&del, 1);
    delayUsInit(&del_read, 1);
    dispInit();
    bool st = false;
    if(uartInit(huart) && nrfInit(hspi)){
        nrfSetModeRX(addr, NRF_CH, sizeof(payload_t));
        dispWrite(0);
        state = RECEIVE;
        st = true;
        snprintf((char * ) buff, sizeof(buff), "\nSistema online\r");
        uartSendString(buff);
        snprintf((char*)buff, sizeof(buff), "CFG=%02X CH=%02X PW=%02X\r\n",
                 nrfReadReg(NRF_CONFIG), nrfReadReg(NRF_RF_CH), nrfReadReg(NRF_RX_PW_P0));
        uartSendString(buff);
    }
	if(nrfReadReg(NRF_CONFIG) != 0x0F)
		NVIC_SystemReset();			// Si el módulo no ha sido configurado correctamente, reinicio TODO el micro
    return st;
}

void parserUpdate(){
	switch(state){
	case RECEIVE:{
		if(nrfRecept((uint8_t*)&payload, sizeof(payload_t))){
			state = UART;
			delayUsSet(&del_read, READ_UPD);
		}
		else
			if(delayUsRead(&del_read)){		//Si en 10 segundos no recibo nada, limpio
				payload.nivel = 0;
				delayUsSet(&del_read, READ_UPD);
				state = PRINT_LEV;
			}
			else
				state =  PRINT_LEV;
	}; break;
	case UART:{
		snprintf((char * ) buff, sizeof(buff), "\nMedicion: %u \r", payload.nivel);
		uartSendString(buff);
		state = PRINT_LEV;
	}; break;
	case PRINT_LEV:{
		measureLevel(payload.nivel);
		updateState();		//Actualizo display
		state = RECEIVE;
	}; break;
	};//sw
}

void debugSpi(){
    snprintf((char*)buff, sizeof(buff), "CFG=%02X CH=%02X PW=%02X\r\n",
    nrfReadReg(NRF_CONFIG), nrfReadReg(NRF_RF_CH), nrfReadReg(NRF_RX_PW_P0));
    uartSendString(buff);
}
