/*
 * ports_slave.h
 *
 *  Created on: 18 abr 2026
 *      Author: matute
 */

#ifndef DRIVERS_API_INC_PORTS_SLAVE_H_
#define DRIVERS_API_INC_PORTS_SLAVE_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
	#include "stm32f1xx_hal.h"
	#include "nrf24l01_ports.h"
	#include "jsnsr04_ports.h"
	#include "yfs201_ports.h"
	#define LED_GREEN_Pin GPIO_PIN_13
	#define LED_GREEN_GPIO_Port GPIOC

	#define BTN_Pin GPIO_PIN_11
	#define BTN_GPIO_Port GPIOB
	#define BTN_EXTI_IRQn EXTI15_10_IRQn
	#define DHT22_Pin GPIO_PIN_9
	#define DHT22_GPIO_Port GPIOB

#endif



#endif /* DRIVERS_API_INC_PORTS_SLAVE_H_ */
