/*
 * API_parser.h
 *
 *  Created on: 11 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *                    MASTER
 */

#ifndef DRIVERS_API_INC_API_PARSER_H_
#define DRIVERS_API_INC_API_PARSER_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include "API_display.h"
#include "nrf24l01.h"
#include "uart_mx.h"
#include <stdio.h>
#include <stdbool.h>
#include "u_delay.h"

bool initParser(UART_HandleTypeDef *huart, SPI_HandleTypeDef *hspi);
void parserUpdate(void);
void measureLevel(uint16_t level);
void debugSpi();


#endif /* DRIVERS_API_INC_API_PARSER_H_ */
