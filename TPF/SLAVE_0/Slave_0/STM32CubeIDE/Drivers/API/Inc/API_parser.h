/*
 * API_parser.h
 *
 *  Created on: 5 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *      SLAVE
 */

#ifndef DRIVERS_API_INC_API_PARSER_H_
#define DRIVERS_API_INC_API_PARSER_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif

#include <stdbool.h>
#include "nrf24l01.h"
#include "jsnsr04t.h"
#include <string.h>
#include "uart_mx.h"


bool parserInit(UART_HandleTypeDef *huart, SPI_HandleTypeDef *hspi, TIM_HandleTypeDef *tim, uint8_t channel, GPIO_TypeDef *trig_port, uint16_t trig_pin);

void updateParser();


#endif /* DRIVERS_API_INC_API_PARSER_H_ */
