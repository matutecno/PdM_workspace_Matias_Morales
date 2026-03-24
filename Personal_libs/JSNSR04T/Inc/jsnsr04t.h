/*
 * jsnsr04t.h
 *
 *  Created on: 22 mar 2026
 *      Author: Matías Morales Gariglio
 */

#ifndef JSNSR04T_INC_JSNSR04T_H_
#define JSNSR04T_INC_JSNSR04T_H_

#include "jsnsr04t_port.h"
#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define SP_SOUND 0.0343		//Velocidad del sonido, 343 m/s, 0,0343 cm/uS

typedef struct {
    float distance_cm;
    uint32_t trise;
    uint32_t tfall;
    bool first_capture;
} JSNSR04T_t;

// Prototipos de funciones
void JSNSR04T_Init(void);
void JSNSR04T_Trigger(void);
void JSNSR04T_Callback(TIM_HandleTypeDef * htim);
float JSNSR04T_GetDistance(void);

#endif /* JSNSR04T_INC_JSNSR04T_H_ */
