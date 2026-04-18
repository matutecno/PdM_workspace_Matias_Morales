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
	#define LED_GREEN_Pin GPIO_PIN_13
	#define LED_GREEN_GPIO_Port GPIOC
	#define JSN_TRIG_Pin GPIO_PIN_1
	#define JSN_TRIG_GPIO_Port GPIOA
	#define NRF_CE_Pin GPIO_PIN_3
	#define NRF_CE_GPIO_Port GPIOA
	#define NRF_CSN_Pin GPIO_PIN_4
	#define NRF_CSN_GPIO_Port GPIOA
	#define NRF_IRQ_Pin GPIO_PIN_0
	#define NRF_IRQ_GPIO_Port GPIOB
	#define NRF_IRQ_EXTI_IRQn EXTI0_IRQn
	#define FLOW_SENSOR_Pin GPIO_PIN_10
	#define FLOW_SENSOR_GPIO_Port GPIOB
	#define FLOW_SENSOR_EXTI_IRQn EXTI15_10_IRQn
	#define BTN_Pin GPIO_PIN_11
	#define BTN_GPIO_Port GPIOB
	#define BTN_EXTI_IRQn EXTI15_10_IRQn
	#define DHT22_Pin GPIO_PIN_9
	#define DHT22_GPIO_Port GPIOB

#endif



#endif /* DRIVERS_API_INC_PORTS_SLAVE_H_ */
