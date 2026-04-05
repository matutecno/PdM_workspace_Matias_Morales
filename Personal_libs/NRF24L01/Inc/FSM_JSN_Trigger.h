/*
 * FSM_JSN_Trigger.h
 *
 *  Created on: 30 mar 2026
 *      Author: Matías Morales
 */

#ifndef DRIVERS_API_INC_FSM_JSN_TRIGGER_H_
#define DRIVERS_API_INC_FSM_JSN_TRIGGER_H_

typedef enum{
	STATE_H,
	STATE_L,
}st_jsn_trig;


void updateStateTrigger();
void initTrigger();


#endif /* DRIVERS_API_INC_FSM_JSN_TRIGGER_H_ */
