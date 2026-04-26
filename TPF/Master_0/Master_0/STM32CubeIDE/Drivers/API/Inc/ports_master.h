/*
 * ports_master.h
 *
 *  Created on: 18 abr 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef DRIVERS_API_INC_PORTS_MASTER_H_
#define DRIVERS_API_INC_PORTS_MASTER_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
	#include "stm32f1xx_hal.h"
	#include "nrf24l01_ports.h"
	#define LED_GREEN_Pin GPIO_PIN_13
	#define LED_GREEN_GPIO_Port GPIOC
	#define Pul_0_Pin GPIO_PIN_10
	#define Pul_0_GPIO_Port GPIOB
	#define Pul_0_EXTI_IRQn EXTI15_10_IRQn
	#define Pul_1_Pin GPIO_PIN_11
	#define Pul_1_GPIO_Port GPIOB
	#define Pul_1_EXTI_IRQn EXTI15_10_IRQn
	#define Pul_2_Pin GPIO_PIN_12
	#define Pul_2_GPIO_Port GPIOB
	#define Pul_2_EXTI_IRQn EXTI15_10_IRQn
	#define SER_Pin GPIO_PIN_3
	#define SER_GPIO_Port GPIOB
	#define SRCLK_Pin GPIO_PIN_4
	#define SRCLK_GPIO_Port GPIOB
	#define RCLK_Pin GPIO_PIN_5
	#define RCLK_GPIO_Port GPIOB
	#define SRCLR_Pin GPIO_PIN_6
	#define SRCLR_GPIO_Port GPIOB
	#define OE_Pin GPIO_PIN_7
	#define OE_GPIO_Port GPIOB
	#endif

#endif /* DRIVERS_API_INC_PORTS_MASTER_H_ */
