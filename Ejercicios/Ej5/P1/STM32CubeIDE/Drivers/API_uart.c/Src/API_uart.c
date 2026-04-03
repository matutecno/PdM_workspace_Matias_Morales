/*
 * API_uart.c
 *
 *  Created on: 3 abr 2026
 *      Author: Matías Morales Gariglio
 */


#include "API_uart.h"

static UART_HandleTypeDef UartHandle;

#define sizeOfBuffer	128			//Tamaño del buffer receptor




bool uartInit(){

	static uint8_t arr[15][50]={
			"\n                           ",
			"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
			"\n                           ",
			"\nConfiguracion de terminal serie: ",
			"\nInstancia: USART2 ",
			"\nBaudios: 9600",
			"\nAncho de palabra: 8 bits",
			"\nBits de parada: 1",
			"\nParidad: impar",
			"\nSin control de flujo",
			"\nModo: Tx, Rx",
			"\nOversampling: 16",
			"\n                           ",
			"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
			" \n                          \n\n",
	};

	UartHandle.Instance 			= USART2;
	UartHandle.Init.BaudRate 		= 9600;
	UartHandle.Init.WordLength 		= UART_WORDLENGTH_9B;
	UartHandle.Init.StopBits 		= UART_STOPBITS_1;
	UartHandle.Init.Parity 			= UART_PARITY_ODD;
	UartHandle.Init.HwFlowCtl		= UART_HWCONTROL_NONE;
	UartHandle.Init.Mode 			= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling	= UART_OVERSAMPLING_16;

	if( HAL_UART_Init(&UartHandle) ){
		Error_Handler();
		return 0;
	}

	for(int i = 0; i<15; i++)
		uartSendString(&arr[i]);
	return 1;
};


void uartSendString(uint8_t * pstring){
	if(pstring != NULL)
		HAL_UART_Transmit(&UartHandle, pstring, strlen((char*)pstring), HAL_MAX_DELAY);
};


void uartSendStringSize(uint8_t * pstring, uint16_t size){
	static uint16_t i;
	if(pstring != NULL && size > 0 && size < 256)
		for(i = 0; i < size; i++)
			HAL_UART_Transmit(&UartHandle, &pstring[i], 1, HAL_MAX_DELAY);
};

void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	static uint16_t i = 0;
	if(pstring != NULL && size > 0 && size <= sizeOfBuffer){
		do{
			HAL_UART_Receive(&UartHandle, &pstring[i], 1, HAL_MAX_DELAY);
			i++;
		}while(pstring[i-1] != '\n' && i < size - 1);

	}
	pstring[i] = '\0';
	i = 0;
};



