/*
 * API_display.c
 *
 *  Created on: 2 abr 2026
 *      Author: Matías Morales Gariglio
 *
 *      		MASTER
 */

#include "API_display.h"
#include "ports_master.h"

#define sizeOfData	15
#define BUZZ_ON 100000
#define BUZZ_OFF 2000000

static state_t state;
static uint16_t ht = 55, hs = 5;			//ht Altura total del tanque, hs Altura del sensor al agua, estando el tanque lleno
static delUs_t del_buz;						//Para hacer un beep en vez de que suene ctemente
bool buz_on;								//Para hacer un beep en vez de que suene ctemente

void dispInit(){
	state = STATE_NULL;
	delayUsInit(&del_buz, 1);
}

void updateState(){

	switch (state){
		case STAT_W0:{
			dispWrite(DISP_L0 | DISP_LED_WAT);
		};break;
		case STAT_W1:{
			dispWrite(DISP_L0 | DISP_L1| DISP_LED_WAT);
		};break;
		case STAT_W2:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2| DISP_LED_WAT);
		};break;
		case STAT_W3:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3| DISP_LED_WAT);
		};break;
		case STAT_W4:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4| DISP_LED_WAT);
		};break;
		case STAT_W5:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5| DISP_LED_WAT);
		};break;
		case STAT_W6:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5 | DISP_L6| DISP_LED_WAT);
		};break;
		case STAT_W7:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5 | DISP_L6 | DISP_L7| DISP_LED_WAT);
		};break;
		case STAT_W8:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5 | DISP_L6 | DISP_L7 | DISP_L8| DISP_LED_WAT);
		};break;
		case STAT_W9:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5 | DISP_L6 |
					DISP_L7 | DISP_L8 | DISP_L9| DISP_LED_WAT);
		};break;
		case STAT_B0:{
			dispWrite(DISP_L0 | DISP_LED_BAT);
		};break;
		case STAT_B1:{
			dispWrite(DISP_L0 | DISP_L1| DISP_LED_BAT);
		};break;
		case STAT_B2:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2| DISP_LED_BAT);
		};break;
		case STAT_B3:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3| DISP_LED_BAT);
		};break;
		case STAT_B4:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4| DISP_LED_BAT);
		};break;
		case STAT_B5:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5| DISP_LED_BAT);
		};break;
		case STAT_B6:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 |
					DISP_L5 | DISP_L6| DISP_LED_BAT);
		};break;
		case STAT_B7:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 |
					DISP_L5 | DISP_L6 | DISP_L7| DISP_LED_BAT);
		};break;
		case STAT_B8:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 |
					DISP_L5 | DISP_L6 | DISP_L7 | DISP_L8| DISP_LED_BAT);
		};break;
		case STAT_B9:{
			dispWrite(DISP_L0 | DISP_L1 | DISP_L2 | DISP_L3 | DISP_L4 | DISP_L5 | DISP_L6 |
					DISP_L7 | DISP_L8 | DISP_L9| DISP_LED_BAT);
		};break;
		case STAT_W0_BUZZ:{
			dispWrite(DISP_L0 | DISP_LED_WAT | DISP_BUZZ);
		};break;
		case STAT_W1_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_LED_WAT | DISP_BUZZ);
		};break;
		case STAT_W2_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_L2 | DISP_LED_WAT | DISP_BUZZ);
		};break;
		case STAT_W3_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_L2 |  DISP_L3 |DISP_LED_WAT | DISP_BUZZ);
		};break;
		case STAT_B0_BUZZ:{
			dispWrite(DISP_L0 | DISP_LED_BAT | DISP_BUZZ);
		};break;
		case STAT_B1_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_LED_BAT | DISP_BUZZ);
		};break;
		case STAT_B2_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_L2 | DISP_LED_BAT | DISP_BUZZ);
		};break;
		case STAT_B3_BUZZ:{
			dispWrite(DISP_L0 | DISP_L1 |  DISP_L2 |  DISP_L3 |DISP_LED_BAT | DISP_BUZZ);
		};break;
		case STATE_NULL : dispWrite(0); break;
		default: dispWrite(0);
	}
};

void dispWrite(uint16_t data){
	int8_t i = 0;
	for(i = 0; i <= sizeOfData; i++){
		HAL_GPIO_WritePin(SER_GPIO_Port, SER_Pin, (data >> i) & 1);
		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SRCLK_GPIO_Port, SRCLK_Pin, GPIO_PIN_RESET);
	};
	  HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(RCLK_GPIO_Port, RCLK_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(OE_GPIO_Port, OE_Pin, GPIO_PIN_RESET);
};

void measureLevel(uint16_t nivel){
	uint8_t lev = 0;

	if(nivel >= ht || nivel == 0)
		state = STATE_NULL;
	else{

		lev = (ht - nivel) * 10 / (ht - hs);

		switch(lev){
		case 10: state = STAT_W9; break;
		case 9: state = STAT_W8; break;
		case 8: state = STAT_W7; break;
		case 7: state = STAT_W6; break;
		case 6: state = STAT_W5; break;
		case 5: state = STAT_W4; break;
		case 4:{
	      if(delayUsRead(&del_buz)){
	          buz_on = !buz_on;
	          if(buz_on){
	              state = STAT_W3_BUZZ;
	              delayUsSet(&del_buz, BUZZ_ON);
	          } else {
	              state = STAT_W3;
	              delayUsSet(&del_buz, BUZZ_OFF);
	          }
	      }
		}; break;

		case 3: {
		      if(delayUsRead(&del_buz)){
		          buz_on = !buz_on;
		          if(buz_on){
		              state = STAT_W2_BUZZ;
		              delayUsSet(&del_buz, BUZZ_ON);
		          } else {
		              state = STAT_W2;
		              delayUsSet(&del_buz, BUZZ_OFF);
		          }
		      }
			}; break;
		case 2: {
		      if(delayUsRead(&del_buz)){
		          buz_on = !buz_on;
		          if(buz_on){
		              state = STAT_W1_BUZZ;
		              delayUsSet(&del_buz, BUZZ_ON);
		          } else {
		              state = STAT_W1;
		              delayUsSet(&del_buz, BUZZ_OFF);
		          }
		      }
			}; break;
		case 1: {
		      if(delayUsRead(&del_buz)){
		          buz_on = !buz_on;
		          if(buz_on){
		              state = STAT_W0_BUZZ;
		              delayUsSet(&del_buz, BUZZ_ON);
		          } else {
		              state = STAT_W0;
		              delayUsSet(&del_buz, BUZZ_OFF);
		          }
		      }
			}; break;
		case 0: state = STATE_NULL;
		}//sw

	}
};
