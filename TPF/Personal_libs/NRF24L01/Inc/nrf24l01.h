/*
 * nrf24l01.h
 *
 *  Created on: 31 mar 2026
 *      Author: matute
 */

#ifndef DRIVERS_API_INC_NRF24L01_H_
#define DRIVERS_API_INC_NRF24L01_H_

#if defined(STM32F446xx)		//el device defined se obtiene de la info de cada proyecto.
    #include "stm32f4xx_hal.h"
#elif defined(STM32F103xB)
    #include "stm32f1xx_hal.h"
#endif



#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "u_delay.h"

  // Registros
#define NRF_CONFIG      0x00
#define NRF_EN_AA       0x01
#define NRF_EN_RXADDR   0x02
#define NRF_SETUP_AW    0x03
#define NRF_SETUP_RETR  0x04
#define NRF_RF_CH       0x05
#define NRF_RF_SETUP    0x06
#define NRF_STATUS      0x07
#define NRF_RX_ADDR_P0  0x0A
#define NRF_TX_ADDR     0x10
#define NRF_RX_PW_P0    0x11

  // Comandos
#define NRF_CMD_R_REG       0x00
#define NRF_CMD_W_REG       0x20
#define NRF_CMD_R_RX_PLD    0x61
#define NRF_CMD_W_TX_PLD    0xA0
#define NRF_CMD_FLUSH_TX    0xE1	//MOSI recibe 0xE1, vacía la TX_FIFO
#define NRF_CMD_FLUSH_RX    0xE2	//MOSI recibe 0xE2, vacía la RTX_FIFO
#define NRF_CMD_NOP         0xFF

  // Bits del registro STATUS. Con esto hacemos las máscaras para preguntar del registro status (reg 0x07):
#define NRF_STATUS_RX_DR    (1 << 6)	//Data Ready RX FIFO interrupt. Asserted when new data arrives RX FIFO. Write 1 to clear bit
#define NRF_STATUS_TX_DS    (1 << 5)	//Data Sent TX FIFO interrupt. Asserted when packet transmitted on TX. If AUTO_ACK is activated,
										//this bit is set high only when ACK is received.
#define NRF_STATUS_MAX_RT   (1 << 4)	//Maximum number of TX retransmits interrupt. Write 1 to clear bit. If MAX_RT is asserted it must
										//be cleared to enable further communication. Lo usaré para verificar si la comunicación se pudo establecer

#define NRF_CH 100

bool nrfInit(SPI_HandleTypeDef *hspi);
void nrfWriteReg(uint8_t reg, uint8_t val);
uint8_t nrfReadReg(uint8_t reg);
void nrfWriteAddr(uint8_t reg, const uint8_t *addr, uint8_t len);
void nrfSetModeTX(const uint8_t *address, uint8_t channel, uint8_t payload_len);
void nrfSetModeRX(const uint8_t *address, uint8_t channel, uint8_t payload_len);
void nrfTransmit(uint8_t *data, uint8_t len);
bool nrfRecept(uint8_t *data, uint8_t len);
bool nrfIsBusy(void);

typedef struct {
    uint16_t nivel;       // cm
    bool caudal;      // pulsos/seg (o L/min ×10 si convertís)
    uint16_t luminosidad; // ADC raw 12-bit
    int16_t  temperatura; // °C × 10
    uint8_t  humedad;     // %
}payload_t;


#endif

/*
 * Resumen de los registros:
 *
 *┌───────────┬────────────┬───────────────────────────────────────────────────────────┐
  │ Dirección │   Nombre   │                        Descripción                        │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x00      │ CONFIG     │ Configuración general: PWR_UP, PRIM_RX, CRC, máscaras IRQ │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x01      │ EN_AA      │ Habilita auto-acknowledgment por pipe (P0..P5)            │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x02      │ EN_RXADDR  │ Habilita pipes de recepción activos                       │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x03      │ SETUP_AW   │ Ancho de dirección: 3, 4 o 5 bytes                        │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x04      │ SETUP_RETR │ Retransmisiones: delay y cantidad de reintentos           │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x05      │ RF_CH      │ Canal de RF (0–125 → 2.400–2.525 GHz)                     │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x06      │ RF_SETUP   │ Velocidad (250k/1M/2Mbps) y potencia TX                   │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x07      │ STATUS     │ Flags: TX_DS, MAX_RT, RX_DR, TX_FULL, pipe RX activo      │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x08      │ OBSERVE_TX │ Contadores de paquetes perdidos y retransmitidos          │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x09      │ RPD        │ Received Power Detector: detecta señal RF en el canal     │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0A      │ RX_ADDR_P0 │ Dirección del pipe 0 (hasta 5 bytes)                      │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0B      │ RX_ADDR_P1 │ Dirección del pipe 1 (hasta 5 bytes)                      │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0C      │ RX_ADDR_P2 │ Dirección del pipe 2 (1 byte, MSB de P1)                  │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0D      │ RX_ADDR_P3 │ Dirección del pipe 3 (1 byte, MSB de P1)                  │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0E      │ RX_ADDR_P4 │ Dirección del pipe 4 (1 byte, MSB de P1)                  │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x0F      │ RX_ADDR_P5 │ Dirección del pipe 5 (1 byte, MSB de P1)                  │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x10      │ TX_ADDR    │ Dirección del destinatario en modo TX (hasta 5 bytes)     │
  ├───────────┼────────────┼───────────────────────────────────────────────────────────┤
  │ 0x11      │ RX_PW_P0   │ Cantidad de bytes del payload esperado en pipe 0          │
  └───────────┴────────────┴───────────────────────────────────────────────────────────┘

  ┌──────┬────────────┬────────────────────────────────────────────────────────────────────────────────────────────────┐
  │ Dir  │   Nombre   │                                        Bits relevantes                                         │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x00 │ CONFIG     │ [6] MASK_RX_DR, [5] MASK_TX_DS, [4] MASK_MAX_RT, [3] EN_CRC, [2] CRCO, [1] PWR_UP, [0] PRIM_RX │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x01 │ EN_AA      │ [5:0] ENAA_P5..ENAA_P0 — 1 bit por pipe                                                        │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x02 │ EN_RXADDR  │ [5:0] ERX_P5..ERX_P0 — habilita cada pipe                                                      │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x03 │ SETUP_AW   │ [1:0] AW — 01=3B, 10=4B, 11=5B                                                                 │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x04 │ SETUP_RETR │ [7:4] ARD (delay×250µs+250µs), [3:0] ARC (0–15 reintentos)                                     │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x05 │ RF_CH      │ [6:0] RF_CH — canal 0–125                                                                      │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x06 │ RF_SETUP   │ [5] RF_DR_LOW, [3] RF_DR_HIGH, [2:1] RF_PWR — 00=-18dBm, 11=0dBm                               │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x07 │ STATUS     │ [6] RX_DR, [5] TX_DS, [4] MAX_RT, [3:1] RX_P_NO (pipe que recibió), [0] TX_FULL                │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x08 │ OBSERVE_TX │ [7:4] PLOS_CNT (paquetes perdidos), [3:0] ARC_CNT (reintentos actuales)                        │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x09 │ RPD        │ [0] RPD — 1 si potencia recibida > -64 dBm                                                     │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0A │ RX_ADDR_P0 │ [39:0] dirección completa de 5 bytes                                                           │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0B │ RX_ADDR_P1 │ [39:0] dirección completa de 5 bytes                                                           │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0C │ RX_ADDR_P2 │ [7:0] byte menos significativo (MSB heredado de P1)                                            │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0D │ RX_ADDR_P3 │ [7:0] byte menos significativo (MSB heredado de P1)                                            │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0E │ RX_ADDR_P4 │ [7:0] byte menos significativo (MSB heredado de P1)                                            │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x0F │ RX_ADDR_P5 │ [7:0] byte menos significativo (MSB heredado de P1)                                            │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x10 │ TX_ADDR    │ [39:0] dirección destino de 5 bytes                                                            │
  ├──────┼────────────┼────────────────────────────────────────────────────────────────────────────────────────────────┤
  │ 0x11 │ RX_PW_P0   │ [5:0] payload width pipe 0 — 1 a 32 bytes                                                      │
  └──────┴────────────┴────────────────────────────────────────────────────────────────────────────────────────────────┘



 */
