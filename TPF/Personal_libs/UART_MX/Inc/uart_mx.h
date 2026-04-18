/*
 * uart_mx.h
 *
 *  Created on: 10 abr 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef DRIVERS_API_UART_C_INC_API_UART_H_
#define DRIVERS_API_UART_C_INC_API_UART_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "usart.h"
#include <stdio.h>
#include "u_delay.h"

#define CMD_MAX_LINE 64

bool uartInit(UART_HandleTypeDef * uart);
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
int8_t uartReceiveString(uint8_t * pstring);


#endif /* DRIVERS_API_UART_C_INC_API_UART_H_ */
