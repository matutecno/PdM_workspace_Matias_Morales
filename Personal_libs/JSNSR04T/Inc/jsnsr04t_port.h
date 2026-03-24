/*
 * jsonsr04t_port.h
 *
 *  Created on: 22 mar 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef JSNSR04T_INC_JSNSR04T_PORT_H_
#define JSNSR04T_INC_JSNSR04T_PORT_H_

#ifndef JSNS_SR04T_PORT_H_
#define JSNS_SR04T_PORT_H_

#include "main.h" // Importante: main.h trae las definiciones de la HAL
extern TIM_HandleTypeDef htim2;

/* --- CONFIGURACIÓN DE PINES --- */
// Si cambias de placa, solo modificas estos alias:

#define JSN_TRIG_PORT    GPIOA
#define JSN_TRIG_PIN     GPIO_PIN_0

#define JSN_ECHO_PORT    GPIOA
#define JSN_ECHO_PIN     GPIO_PIN_1

#define JSN_TIMER_HANDLE htim2
#define JSN_TIMER_CH     TIM_CHANNEL_2

#endif /* JSNS_SR04T_PORT_H_ */



#endif /* JSNSR04T_INC_JSNSR04T_PORT_H_ */
