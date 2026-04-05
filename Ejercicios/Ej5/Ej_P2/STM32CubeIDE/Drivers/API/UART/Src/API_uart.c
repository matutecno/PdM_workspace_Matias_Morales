/*
 * API_uart.c
 *
 *  Created on: 3 abr 2026
 *      Author: Matías Morales Gariglio
 */

#include "API_cmdparser.h"
#include "API_uart.h"

static UART_HandleTypeDef UartHandle;


#define sizeOfBuffer	128			//Tamaño del buffer receptor

bool uartInit(){

	static uint8_t buf_baud[32];
	static uint8_t buf_word[32];
	static uint8_t buf_stop[32];
	static uint8_t buf_par[32];
	static uint8_t buf_over[32];

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

	snprintf((char*)buf_baud, sizeof(buf_baud), "\nBaudios: %lu",       	UartHandle.Init.BaudRate);
	snprintf((char*)buf_word, sizeof(buf_word), "\nAncho de palabra: %s", (UartHandle.Init.WordLength == UART_WORDLENGTH_9B) ? "9 bits" : "8 bits");
	snprintf((char*)buf_stop, sizeof(buf_stop), "\nBits de parada: %s",   (UartHandle.Init.StopBits  == UART_STOPBITS_1)    ? "1"      : "2");
	snprintf((char*)buf_par,  sizeof(buf_par),  "\nParidad: %s",          (UartHandle.Init.Parity    == UART_PARITY_ODD)    ? "Impar"  : "Par");
	snprintf((char*)buf_over, sizeof(buf_over), "\nOversampling: %s",     (UartHandle.Init.OverSampling == UART_OVERSAMPLING_16) ? "16" : "8");

	uint8_t *arr[] = {
		(uint8_t *)"\n                           ",
	    (uint8_t *)"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
	    (uint8_t *)"\nConfiguracion inicial de terminal serie: ",
	    (uint8_t *)"\nInstancia: USART2",
	    buf_baud,
	    buf_word,
	    buf_stop,
	    buf_par,
	    (uint8_t *)"\nSin control de flujo",
	    (uint8_t *)"\nModo: Tx, Rx",
	    buf_over,
	    (uint8_t *)"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n\n"
	  };

	for(int i = 0; i < (int)(sizeof(arr)/sizeof(arr[0])); i++)
		uartSendString(arr[i]);
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

int8_t uartReceiveString(uint8_t * pstring){
	static uint16_t i = 0;
	int8_t ret = 1;
	if(pstring != NULL){
		do{
			HAL_StatusTypeDef status = HAL_UART_Receive(&UartHandle, &pstring[i], 1, 1);		//Es para evitar que Rx uart sea bloqueante

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



