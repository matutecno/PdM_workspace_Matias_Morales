/*
 * nrf24l01_ports.h
 *
 *  Created on: 26 abr 2026
 *      Author: matute
 */

#ifndef DRIVERS_API_PERSONAL_LIBS_NRF24L01_INC_NRF24L01_PORTS_H_
#define DRIVERS_API_PERSONAL_LIBS_NRF24L01_INC_NRF24L01_PORTS_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
	#include "stm32f1xx_hal.h"
	#define NRF_CE_Pin GPIO_PIN_3
	#define NRF_CE_GPIO_Port GPIOA
	#define NRF_CSN_Pin GPIO_PIN_4
	#define NRF_CSN_GPIO_Port GPIOA
	#define NRF_IRQ_Pin GPIO_PIN_0
	#define NRF_IRQ_GPIO_Port GPIOB
	#define NRF_IRQ_EXTI_IRQn EXTI0_IRQn

#endif /* DRIVERS_API_INC_PORTS_MASTER_H_ */







#endif /* DRIVERS_API_PERSONAL_LIBS_NRF24L01_INC_NRF24L01_PORTS_H_ */
