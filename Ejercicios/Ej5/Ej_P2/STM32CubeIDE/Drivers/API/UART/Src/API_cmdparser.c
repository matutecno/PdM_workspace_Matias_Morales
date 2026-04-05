/*
 * API_debounce.c
 *
 *  Created on: 4 abr 2026
 *      Author: Matías Morales
 */

#include "API_cmdparser.h"
#include "API_uart.h"

typedef enum{
	LED_ON,
	LED_OFF,
	LED_BLINKING
}led_stat_t;

led_blink_t led;
cmd_status_t state;
led_stat_t status_led = LED_OFF;

static uint8_t _tx_buffer[CMD_MAX_LINE]={};

uint8_t * opc[] = {
	(uint8_t *)"LED ON",			//0
	(uint8_t *)"LED OFF",			//1
	(uint8_t *)"LED BLINK",			//2
	(uint8_t *)"STATUS",			//3
	(uint8_t *)"HELP"				//4

};

static uint8_t * mensajes[]={
		(uint8_t *)"\n",
		(uint8_t *)"\n%%%%%%%%%%%%   Comandos de ayuda   %%%%%%%%%%%%",
		(uint8_t *)"\n",
		(uint8_t *)"\nLED ON -> enciende el led de la placa",
		(uint8_t *)"\nLED OFF -> enciende el led de la placa",
		(uint8_t *)"\nLED BLINK -> Hace parpadear al led",
		(uint8_t *)"\nSTATUS -> Retorna el status del sistema ",
		(uint8_t *)"\nHELP -> Retorna el status del sistema ",
		(uint8_t *)"\n",
		(uint8_t *)"\n%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%",
		(uint8_t *)"\n",
};

void cmdParserInit(void){
	state = CMD_PRINT_HELP;
	initLed(&led, 400000);
};

void updateParser(){
	updateLed(&led);
	switch(state){
		case CMD_PRINT_HELP:{
			for(int i = 0; i < ( sizeof(mensajes)/sizeof(mensajes[0]) ) ; i++)
				uartSendString(mensajes[i]);
			state = CMD_IDLE;
		}; break;

		case CMD_IDLE:{
			uint8_t * m0 = (uint8_t *)"\n\n$: ";
			uartSendString(m0);
			state = CMD_RECEIVING;
		};break;

		case CMD_ECHO:{
			uint8_t * m0 = (uint8_t *)"\nString recibido: ";
			uartSendString(m0);
			uartSendString(_tx_buffer);
			state = CMD_PROCESS;
		};break;

		case CMD_ERR_OVERFLOW:{
			uint8_t m0[50]={};
			snprintf((char *) m0, sizeof(m0), "\nError: Overflow. Maximo permitido: %d", CMD_MAX_LINE );
			uartSendString(m0);
		}; break;

		case CMD_PROCESS:{
			bool ban = false;
		    int len = strlen((char*)_tx_buffer), i = 0;
		    if(len > 0 && (_tx_buffer[len-1] == '\r' || _tx_buffer[len-1] == '\n'))
		    	_tx_buffer[len-1] = '\0'; 	//Borramos el caracter final, da conflicto con el strcmp el final de cadena

			for(i = 0; i < strlen((char*)_tx_buffer); i++)
				_tx_buffer[i] = toupper(_tx_buffer[i]);

			for(i = 0; i < sizeof(opc)/sizeof(opc[0]); i++){
				if(!strcmp( (char*) _tx_buffer, (char*) opc[i]) ){
					state = CMD_EXEC;
					ban = true;
				};
			};
			if(ban == false){
				state = CMD_ERR_UNKNOWN;
			};
		}; break;

		case CMD_ERR_SYNTAX:{
			uint8_t * m0 = (uint8_t *) "\nLas opciones disponibles son: ";
			uartSendString(m0);
			state = CMD_PRINT_HELP;

		}; break;

		case CMD_ERR_UNKNOWN:{
			uint8_t * m0 = (uint8_t *) "\nOpcion ingresada incorrecta";
			uartSendString(m0);
			state = CMD_ERR_SYNTAX;
		}; break;

		case CMD_RECEIVING:{
			switch(uartReceiveString(_tx_buffer)){
			case -1 :{
				state = CMD_ERR_OVERFLOW;
			}; break;
			case 0:{
				state = CMD_RECEIVING;
			}; break;
			case 1: {
				state = CMD_ECHO;
			}; break;
			}
		}; break;

		case CMD_EXEC:{
			int i = 0;
			for(i = 0; i < sizeof(opc)/sizeof(opc[0]); i++){
				if(!strcmp( (char*) _tx_buffer, (char*) opc[i]) ){
					switch(i){			//el sub i se corresponde a la posición de opc[]
						case 0: {
							ledBlink(false);
							ledOn();
							uint8_t * m0 = (uint8_t *) "\nLed encendido";
							uartSendString(m0);
							state = CMD_IDLE;
							status_led = LED_ON;
						}; break;
						case 1:{
							ledOff();
							uint8_t * m0 = (uint8_t *) "\nLed apagado";
							uartSendString(m0);
							state = CMD_IDLE;
							status_led = LED_OFF;
							ledBlink(false);
						};break;
						case 2:{
							uint8_t * m0 = (uint8_t *) "\nLed blinkeando";
							uartSendString(m0);
							status_led = LED_BLINKING;
							ledBlink(true);
							state = CMD_IDLE;
						};break;
						case 3:{
							switch(status_led){
							case LED_ON:{
								uint8_t * m0 = (uint8_t *) "\nSTATUS: Led ON\n\n";
								uartSendString(m0);
							};break;
							case LED_OFF:{
								uint8_t * m0 = (uint8_t *) "\nSTATUS: Led OFF\n\n";
								uartSendString(m0);
							}; break;
							case LED_BLINKING:{
								uint8_t * m0 = (uint8_t *) "\nSTATUS: Led BLINK\n\n";
								uartSendString(m0);
							}; break;
							}
							state = CMD_IDLE;
						};break;
						case 4:{
							state = CMD_PRINT_HELP;
						}; break;
					}//sw i
				}//if
			};//for
		}; break;//sw CMD_CASE


	}//switch state
}//funcion updateParser;

void debug(){
	uint8_t * m0 = (uint8_t *) "* * * * * * *";
	uartSendString(m0);
}

void ledOn(){
	led.flame = true;
}

void ledOff(){
	led.flame = false;
	led.run = false;
}

void ledBlink(bool st){
	led.flame = false;
	led.run = st;
}

void updateLed(){
	updateStateLed(&led);
};
