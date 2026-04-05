/*
 * API_debounce.h
 *
 *  Created on: 4 abr 2026
 *      Author: Matías Morales
 */

#ifndef DRIVERS_API_UART_C_INC_API_CMDPARSER_H_
#define DRIVERS_API_UART_C_INC_API_CMDPARSER_H_

#define CMD_MAX_LINE	64
#define CMD_MAX_TOKENS 	3

#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include "led_blink.h"

typedef enum{
	CMD_OK = 0,
	CMD_ERR_OVERFLOW,
	CMD_ERR_SYNTAX,
	CMD_ERR_UNKNOWN,
	CMD_ERR_ARG,
	CMD_PROCESS,
	CMD_EXEC,
	CMD_IDLE,
	CMD_RECEIVING,
	CMD_PRINT_HELP,
	CMD_ECHO
}cmd_status_t;

void updateParser(void);
void cmdParserInit(void);
void cmdPoll(void);
void cmdPrintHelp(void);
void ledOn(void);
void ledOff(void);
void ledBlink(bool st);
void debug(void);
void updateLed();



#endif /* DRIVERS_API_UART_C_INC_API_CMDPARSER_H_ */
