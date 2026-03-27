/*
 * project_port.h
 *
 *  Created on: 22 mar 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef JSNSR04T_INC_PROJECT_PORT_H_
#define JSNSR04T_INC_PROJECT_PORT_H_

#include "main.h" // Importante: main.h trae las definiciones de la HAL

extern TIM_HandleTypeDef  htim2;

#ifdef HAL_SPI_MODULE_ENABLED
extern SPI_HandleTypeDef  hspi1;
#endif

#ifdef HAL_ADC_MODULE_ENABLED
extern ADC_HandleTypeDef  hadc1;
#endif

/* --- JSN-SR04T --- */
#define JSN_TRIG_PORT    GPIOA
#define JSN_TRIG_PIN     GPIO_PIN_1

#define JSN_ECHO_PORT    GPIOA
#define JSN_ECHO_PIN     GPIO_PIN_0

#define JSN_TIMER_HANDLE htim2
#define JSN_TIMER_CH     TIM_CHANNEL_1

/* --- nRF24L01+ --- */
#define NRF_SPI_HANDLE   hspi1

#define NRF_CSN_PORT     GPIOA
#define NRF_CSN_PIN      GPIO_PIN_4

#define NRF_CE_PORT      GPIOA
#define NRF_CE_PIN       GPIO_PIN_3

#define NRF_IRQ_PORT     GPIOB
#define NRF_IRQ_PIN      GPIO_PIN_0

/* --- DHT22 --- */
#define DHT_DATA_PORT    GPIOB
#define DHT_DATA_PIN     GPIO_PIN_9

/* --- ADC --- */
#define ADC_HANDLE       hadc1

#define ADC_CH_VBAT      ADC_CHANNEL_2   // PA2 - V_Sense batería
#define ADC_CH_EXTRA     ADC_CHANNEL_9   // PB1 - Pruebas/ADC

#endif /* JSNSR04T_INC_PROJECT_PORT_H_ */
