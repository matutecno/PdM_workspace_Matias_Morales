/*
 * API_uart.c
 *
 *  Created on: 3 abr 2026
 *      Author: Matías Morales Gariglio
 */

#include "uart_mx.h"

#define sizeOfBuffer	128			//Tamaño del buffer receptor
#define TIMEOUT 10000

static UART_HandleTypeDef * UartHandle = NULL;
static delUs_t del_uart;

bool uartInit(UART_HandleTypeDef * uart){
	delayUsInit(&del_uart, 0);
	if(uart != NULL){
		UartHandle = uart;
		return 1;
	}else
		return 0;

}

void uartSendString(uint8_t * pstring){
    if(pstring != NULL){
    	delayUsSet(&del_uart, TIMEOUT);
        while(UartHandle->gState != HAL_UART_STATE_READY && !delayUsRead(&del_uart));
        HAL_UART_Transmit_DMA(UartHandle, pstring, strlen((char*)pstring));
    }
}



int8_t uartReceiveString(uint8_t * pstring){
	static uint16_t i = 0;
	int8_t ret = 1;
	if(pstring != NULL){
		do{
			HAL_StatusTypeDef status = HAL_UART_Receive(UartHandle, &pstring[i], 1, 1);		//Es para evitar que Rx uart sea bloqueante

			if(status == HAL_TIMEOUT)
				return 0;											//Para evitar bloqueo de uart y funcione el blink
			i++;
			if(i >= CMD_MAX_LINE){
				ret = -1;
				break;
			}
		}while(pstring[i-1] != '\n' && pstring[i-1] != '\r' && i <= CMD_MAX_LINE);
		pstring[i] = '\0';
		i = 0;
		return ret;
	}else
		return 0;

	/*
	 *   1 = línea completa recibida
  	  	 0 = todavía esperando (no llegó nada en el timeout)
  	  	 -1 = overflow
	 *
	*/
};


