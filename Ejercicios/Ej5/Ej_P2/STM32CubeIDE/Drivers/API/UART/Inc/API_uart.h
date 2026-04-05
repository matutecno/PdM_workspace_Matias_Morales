/*
 * API_uart.h
 *
 *  Created on: 3 abr 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef DRIVERS_API_UART_C_INC_API_UART_H_
#define DRIVERS_API_UART_C_INC_API_UART_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "usart.h"
#include <stdio.h>

bool uartInit();
void uartSendString(uint8_t * pstring);
void uartSendStringSize(uint8_t * pstring, uint16_t size);
int8_t uartReceiveString(uint8_t * pstring);




#endif /* DRIVERS_API_UART_C_INC_API_UART_H_ */
