/*
 * jsnsr04t.c
 *
 *  Created on: 22 mar 2026
 *      Author: Matías Morales Gariglio
 */


#include "jsnsr04t.h"
#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx_hal.h"

static JSNSR04T_t sensor = {0};

void JSNSR04T_Init(void) {
    HAL_TIM_IC_Start_IT(&JSN_TIMER_HANDLE, JSN_TIMER_CH);
}

void JSNSR04T_Trigger(void) {
    HAL_GPIO_WritePin(JSN_TRIG_PORT, JSN_TRIG_PIN, GPIO_PIN_SET);
    HAL_Delay(1); // 1ms es más que suficiente para el pulso de 10us. Cambiar por retardo no bloqueante
    HAL_GPIO_WritePin(JSN_TRIG_PORT, JSN_TRIG_PIN, GPIO_PIN_RESET);
}

void JSNSR04T_Callback(TIM_HandleTypeDef * htim) {
    if (htim->Instance == JSN_TIMER_HANDLE.Instance) {		//Solo procesamos la interrupcion si viene del timer conectado al sensor TIM2
        if (sensor.first_capture == false) {
            sensor.trise = HAL_TIM_ReadCapturedValue(htim, JSN_TIMER_CH);
            sensor.first_capture = true;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, JSN_TIMER_CH, TIM_INPUTCHANNELPOLARITY_FALLING);
        } else {
            sensor.tfall = HAL_TIM_ReadCapturedValue(htim, JSN_TIMER_CH);

            uint32_t echo = (sensor.tfall > sensor.trise) ? (sensor.tfall - sensor.trise) : (65535 - sensor.trise + sensor.tfall);

            sensor.distance_cm = (float)echo * SP_SOUND / 2.0;
            sensor.first_capture = false;
            __HAL_TIM_SET_CAPTUREPOLARITY(htim, JSN_TIMER_CH, TIM_INPUTCHANNELPOLARITY_RISING);
        }
    }
}

float JSNSR04T_GetDistance(void) {
    return sensor.distance_cm;
}
