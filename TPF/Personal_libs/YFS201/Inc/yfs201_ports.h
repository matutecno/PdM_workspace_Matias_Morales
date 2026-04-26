/*
 * yfs201_ports_slave.h
 *
 *  Created on: 26 abr 2026
 *      Author: matute
 */

#ifndef DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_PORTS_SLAVE_H_
#define DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_PORTS_SLAVE_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
	#include "stm32f1xx_hal.h"
	#define FLOW_SENSOR_Pin GPIO_PIN_10
	#define FLOW_SENSOR_GPIO_Port GPIOB
	#define FLOW_SENSOR_EXTI_IRQn EXTI15_10_IRQn
#endif



#endif /* DRIVERS_API_PERSONAL_LIBS_YFS201_INC_YFS201_PORTS_SLAVE_H_ */
