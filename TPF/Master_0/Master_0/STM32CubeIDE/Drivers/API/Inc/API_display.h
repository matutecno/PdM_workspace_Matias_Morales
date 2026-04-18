/*
 * API_display.h
 *
 *  Created on: 2 abr 2026
 *      Author: Matías Morales Gariglio
 */


#ifndef DRIVERS_API_INC_API_DISPLAY_H_
#define DRIVERS_API_INC_API_DISPLAY_H_

#include "ports_master.h"
#include <stdint.h>
#include <stdbool.h>
#include "u_delay.h"

#define DISP_BUZZ 		(1U << 12)
#define DISP_LED_BAT	(1U << 11)
#define DISP_LED_WAT	(1U << 10)
#define DISP_L0			(1U << 9)
#define DISP_L1			(1U << 8)
#define DISP_L2 		(1U << 7)
#define DISP_L3			(1U << 6)
#define DISP_L4			(1U << 5)
#define DISP_L5			(1U << 4)
#define DISP_L6			(1U << 3)
#define DISP_L7			(1U << 2)
#define DISP_L8			(1U << 1)
#define DISP_L9			(1U << 0)

typedef enum{
	STAT_W0,
	STAT_W0_BUZZ,
	STAT_W1,
	STAT_W1_BUZZ,
	STAT_W2,
	STAT_W2_BUZZ,
	STAT_W3,
	STAT_W3_BUZZ,
	STAT_W4,
	STAT_W5,
	STAT_W6,
	STAT_W7,
	STAT_W8,
	STAT_W9,
	STAT_B0,
	STAT_B0_BUZZ,
	STAT_B1,
	STAT_B1_BUZZ,
	STAT_B2,
	STAT_B2_BUZZ,
	STAT_B3,
	STAT_B3_BUZZ,
	STAT_B4,
	STAT_B5,
	STAT_B6,
	STAT_B7,
	STAT_B8,
	STAT_B9,
	STATE_NULL
}state_t;


void dispInit(void);
void dispWrite(uint16_t dato);
void updateState();

#endif /* DRIVERS_API_INC_API_DISPLAY_H_ */
