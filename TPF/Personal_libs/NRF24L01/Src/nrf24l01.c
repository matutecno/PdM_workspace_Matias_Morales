/*
 * nrf24l01.c
 *
 *  Created on: 31 mar 2026
 *      Author: Matías Morales Gariglio
 */

#include "nrf24l01.h"
#include "nrf24l01_ports.h"

#define TIMEOUT 100



static delUs_t del_TIMEOUT;		//Para probar lo puse en modo bloqueantre

static SPI_HandleTypeDef *_hspi;
static volatile bool _busy = false;

static uint8_t _tx_buf[33];  // 1 cmd + 32 payload max, buffer de transmisión
static uint8_t _rx_buf[33];


//  Control de CSN:
static void csnLow(void)  { HAL_GPIO_WritePin(NRF_CSN_GPIO_Port, NRF_CSN_Pin, GPIO_PIN_RESET); };
static void csnHigh(void) { HAL_GPIO_WritePin(NRF_CSN_GPIO_Port, NRF_CSN_Pin, GPIO_PIN_SET);   };
static void ceHigh(void)  { HAL_GPIO_WritePin(NRF_CE_GPIO_Port,  NRF_CE_Pin,  GPIO_PIN_SET);   };
static void ceLow(void)   { HAL_GPIO_WritePin(NRF_CE_GPIO_Port,  NRF_CE_Pin,  GPIO_PIN_RESET); };

static void _waitBusy(void) {
	delayUsSet(&del_TIMEOUT, TIMEOUT);
    while (_busy && !delayUsRead(&del_TIMEOUT)){};
	if (_busy){
		_busy = false;
		csnHigh();
	};
}

//El callback (va en nrf24l01.c o en stm32f1xx_it.c/main.c en USER CODE):
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef * hspi) {		//Callback de fin de transferencia SPI Tx=RX. Es automático. Cuando el dma
	csnHigh();													//termina de transmitir, ejecuta esto
    _busy = false;
}

bool nrfInit(SPI_HandleTypeDef * hspi) {			//Inicialización del driver.
    _hspi = hspi;								//Guarda el handler
    ceLow();									//Entra en modo stand by
    csnHigh();									//Desactiva el SPI
    HAL_Delay(100);								// power-on reset time (Tpd2stby = 100ms min). Según la hoja de datos requiere 100 mS de estabilización de la fuente
    delayUsSet(&del_TIMEOUT, 0);
    return 1;
}

void nrfSetModeTX(const uint8_t *addr, uint8_t channel, uint8_t payload_len) {
    ceLow();

    nrfWriteReg(NRF_RF_CH,      channel);       // canal de radio (0-125)
    _waitBusy();
    nrfWriteReg(NRF_RF_SETUP,   0x06);          // 0b00000110 1Mbps, 0dBm, máxima potencia
    _waitBusy();
    nrfWriteReg(NRF_EN_AA,      0x00);          // sin auto-ack (simplificado)
    _waitBusy();
    nrfWriteReg(NRF_SETUP_RETR, 0x00);          // sin retransmisión
    _waitBusy();
    nrfWriteReg(NRF_RX_PW_P0,   payload_len);
    _waitBusy();
    nrfWriteAddr(NRF_TX_ADDR,   addr, 5);
    _waitBusy();
    nrfWriteAddr(NRF_RX_ADDR_P0, addr, 5);  // igual a TX_ADDR para ACK
    _waitBusy();
    nrfWriteReg(NRF_CONFIG,     0x0E);         // 0b00001110 PWR_UP=1, PRIM_RX=0, CRC habilitado, CRC de 1 byte.
    _tx_buf[0] = NRF_CMD_FLUSH_TX;
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, 1);	//Vaciar la FIFO en la inicialización
    _waitBusy();
    HAL_Delay(2);   // tiempo de arranque desde power down
}

void nrfSetModeRX(const uint8_t *addr, uint8_t channel, uint8_t payload_len) {
    ceLow();

    nrfWriteReg(NRF_RF_CH,      channel);       // canal de radio (0-125)
    _waitBusy();
    nrfWriteReg(NRF_RF_SETUP,   0x06);          // 0b00000110 1Mbps, 0dBm, máxima potencia
    _waitBusy();
    nrfWriteReg(NRF_EN_AA,      0x00);          // sin auto-ack (simplificado)
    _waitBusy();
    nrfWriteReg(NRF_SETUP_RETR, 0x00);          // sin retransmisión
    _waitBusy();
    nrfWriteReg(NRF_RX_PW_P0,   payload_len);
    _waitBusy();
    nrfWriteReg(NRF_EN_RXADDR, 0x01);   // habilita pipe 0 (ERX_P0)
    _waitBusy();
    nrfWriteAddr(NRF_RX_ADDR_P0, addr, 5);  // igual a TX_ADDR para ACK
    _waitBusy();
    nrfWriteReg(NRF_CONFIG,     0x0F);         // 0b00001111 PWR_UP=1, PRIM_RX=1, CRC habilitado, CRC de 1 byte. Recepcion
    _tx_buf[0] = NRF_CMD_FLUSH_RX;
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, 1);	//Vaciar la FIFO en la inicialización
    _waitBusy();	//Modificar a no bloqueante de ser necesario. Como es init, lo hace una vez nomas no se si vale la pena
    HAL_Delay(2);   // tiempo de arranque desde power down
    ceHigh();
}

//  Escribir un registro (1 byte):
void nrfWriteReg(uint8_t reg, uint8_t val) {
	_tx_buf[0] = NRF_CMD_W_REG | reg;
    _tx_buf[1] = val;
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, 2);		//Envia datos MOSI, recibe simultáneamente por MISO
      // CSN HIGH se hace en el callback
}

//Leer un registro:
uint8_t nrfReadReg(uint8_t reg) {
    _tx_buf[0] = NRF_CMD_R_REG | reg;			//Escribir el registro CONFIG del nrf
    _tx_buf[1] = NRF_CMD_NOP;					//Con el valor NRF_CMD_NOP
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, 2);
    _waitBusy();          // esperás que el DMA termine
    return _rx_buf[1];      // _rx_buf[0] es el STATUS, [1] es el dato. MODIFICAR A NO BLOQUEANTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!
};

bool nrfRecept(uint8_t * data, uint8_t len){
	uint8_t reg_status = 0;
	reg_status = nrfReadReg(NRF_STATUS);	//Leelmos el bit 6, NRF_STATUS_RX_DR. Si es 1 hay dato en la fifo
	if(reg_status & NRF_STATUS_RX_DR){
		_tx_buf[0] = NRF_CMD_R_RX_PLD;
		_busy = true;
		csnLow();
		HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, len+1);
		//while(_busy);
		_waitBusy();
		for(uint8_t i = 0; i < len; i++){
			data[i] = _rx_buf[i+1];		//Byte 0 es status, se descarta
		}
		nrfWriteReg(NRF_STATUS, NRF_STATUS_RX_DR);
		//while(_busy);
		_waitBusy();
		return 1;
	}else
		return 0;
};

//nrfWriteAddr (dirección de 5 bytes):
void nrfWriteAddr(uint8_t reg, const uint8_t * addr, uint8_t len) {
    _tx_buf[0] = NRF_CMD_W_REG | reg;
    for (uint8_t i = 0; i < len; i++)
    	_tx_buf[i+1] = addr[i];			//Cargamos en el buffer la info
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, len + 1);
}

bool nrfIsBusy(void) {
    return _busy;
}


//Transmitir un payload:
void nrfTransmit(uint8_t *data, uint8_t len) {
    _tx_buf[0] = NRF_CMD_W_TX_PLD;
    for (uint8_t i = 0; i < len; i++) _tx_buf[i+1] = data[i];
    _busy = true;
    csnLow();
    HAL_SPI_TransmitReceive_DMA(_hspi, _tx_buf, _rx_buf, len + 1);
    _waitBusy();
    // pulso en CE para iniciar transmisión (mínimo 10µs)
    ceHigh();
    HAL_Delay(1);
    ceLow();
}

/*
 *
  Valores típicos de los registros:

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro CONFIG %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Tu proyecto — TX, sin ACK

  CONFIG = 0x0E  →  0b00001110
  MASK_RX_DR = 0   interrupciones habilitadas
  MASK_TX_DS = 0
  MASK_MAX_RT= 0
  EN_CRC     = 1   CRC activo
  CRCO       = 1   CRC 2 bytes
  PWR_UP     = 1   encendido
  PRIM_RX    = 0   modo TX

  TX con auto-ack (típico)

  CONFIG = 0x0E  →  igual, solo cambia EN_AA y SETUP_RETR
  La diferencia no está en CONFIG sino en que EN_AA≠0x00 y SETUP_RETR≠0x00.


  RX puro

  CONFIG = 0x0F  →  0b00001111
  PWR_UP  = 1
  PRIM_RX = 1   ← única diferencia
  Además hay que poner CE en HIGH de forma permanente para que el receptor esté escuchando.

  Power down (ahorro de energía)

  CONFIG = 0x0C  →  0b00001100
  PWR_UP  = 0   ← apagado
  PRIM_RX = 0
  El nRF queda en standby consumiendo ~900nA. Para volver a operar hay que poner PWR_UP=1 y esperar 1.5ms.



  Posibilidad a futuro: habilitar interrupciones

  ● Las interrupciones posibles son exactamente las 3 que controlan los bits MASK del registro CONFIG:

  ┌─────────────────────┬────────┬────────────────────────────────────────┐
  │         Bit         │  Flag  │                 Evento                 │
  ├─────────────────────┼────────┼────────────────────────────────────────┤
  │ MASK_RX_DR (bit 6)  │ RX_DR  │ Llegó un paquete al RX FIFO            │
  ├─────────────────────┼────────┼────────────────────────────────────────┤
  │ MASK_TX_DS (bit 5)  │ TX_DS  │ Paquete transmitido (+ ACK si EN_AA=1) │
  ├─────────────────────┼────────┼────────────────────────────────────────┤
  │ MASK_MAX_RT (bit 4) │ MAX_RT │ Se agotaron los reintentos sin ACK     │
  └─────────────────────┴────────┴────────────────────────────────────────┘

  El nRF solo puede generar IRQ por esos 3 eventos, y cada MASK bit simplemente decide si ese evento baja el pin IRQ o no.

  Arrancaremos con estos registros en 0 → todos habilitados, pero en la práctica con EN_AA=0 y siendo TX puro, el único que
  puede dispararse realmente es TX_DS.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro EN_AA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Bit:    	7      6      5      4      3      	2      1      	0
         reserv reserv ENAA_P5 ENAA_P4 ENAA_P3 ENAA_P2 ENAA_P1 ENAA_P0


  Valores posibles

  ┌───────────────────────┬──────────────────────────────────────────────────────────────┐
  │         Valor         │                         Significado                          │
  ├───────────────────────┼──────────────────────────────────────────────────────────────┤
  │ 0x00                  │ Auto-ack deshabilitado en todos los pipes                    │
  ├───────────────────────┼──────────────────────────────────────────────────────────────┤
  │ 0x3F                  │ Auto-ack habilitado en todos los pipes (default al encender) │
  ├───────────────────────┼──────────────────────────────────────────────────────────────┤
  │ 0x01                  │ Solo habilitado en P0                                        │
  ├───────────────────────┼──────────────────────────────────────────────────────────────┤
  │ 0x03                  │ Habilitado en P0 y P1                                        │
  ├───────────────────────┼──────────────────────────────────────────────────────────────┤
  │ cualquier combinación │ bit N = 1 habilita pipe N                                    │
  └───────────────────────┴──────────────────────────────────────────────────────────────┘


  Cuando un pipe tiene EN_AA = 1:
  - El receptor manda ACK automáticamente al recibir un paquete por ese pipe
  - El transmisor espera ese ACK antes de considerar el envío exitoso
  - Si no llega el ACK → se activa MAX_RT (según SETUP_RETR)
  - TX_DS solo se activa cuando llega el ACK

  Cuando ENAA=0 (tu caso):
  - El receptor no manda nada
  - El transmisor considera el envío exitoso apenas termina de transmitir
  - MAX_RT nunca se activa
  - TX_DS se activa al terminar la transmisión

Configuramos al proyecto como FIre and forget. Posibilidad a futuro: hacer ack entre tx y rx

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro EN_RXADDR %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

  Registro EN_RXADDR (dirección 0x02)

  Bit:    7      6      5      4      3      2      1      0
         reserv reserv ERX_P5 ERX_P4 ERX_P3 ERX_P2 ERX_P1 ERX_P0

  Habilita o deshabilita cada pipe de recepción. Un pipe deshabilitado no recibe nada aunque tenga dirección configurada.

  ---
  Valores posibles

  ┌───────┬──────────────────────────────┐
  │ Valor │        Pipes activos         │
  ├───────┼──────────────────────────────┤
  │ 0x00  │ Ninguno (nRF no recibe nada) │
  ├───────┼──────────────────────────────┤
  │ 0x3F  │ Todos (P0–P5)                │
  ├───────┼──────────────────────────────┤
  │ 0x01  │ Solo P0                      │
  ├───────┼──────────────────────────────┤
  │ 0x02  │ Solo P1                      │
  ├───────┼──────────────────────────────┤
  │ 0x03  │ P0 y P1                      │
  └───────┴──────────────────────────────┘

  ---
  Combinaciones típicas TX/RX

  TX puro sin ACK (tu proyecto)

  EN_AA      = 0x00  — sin auto-ack
  EN_RXADDR  = 0x01  — P0 habilitado (o incluso 0x00, no importa)
  CONFIG     = 0x0E  — PRIM_RX=0
  EN_RXADDR no tiene efecto real porque nunca escuchás nada.

  TX con auto-ack

  EN_AA      = 0x01  — auto-ack en P0
  EN_RXADDR  = 0x01  — P0 habilitado para recibir el ACK
  CONFIG     = 0x0E  — PRIM_RX=0
  P0 debe estar habilitado obligatoriamente porque el ACK llega por ahí.

  RX escuchando un transmisor

  EN_AA      = 0x01  — auto-ack en P0
  EN_RXADDR  = 0x01  — P0 habilitado
  CONFIG     = 0x0F  — PRIM_RX=1
  CE         = HIGH  — escuchando continuamente

  RX escuchando 6 transmisores simultáneos

  EN_AA      = 0x3F  — auto-ack en todos
  EN_RXADDR  = 0x3F  — todos los pipes activos
  CONFIG     = 0x0F  — PRIM_RX=1
  CE         = HIGH
  Cada transmisor tiene su propia dirección y el nRF los distingue automáticamente por pipe.

  ---
  Relación entre EN_AA y EN_RXADDR

  Son independientes pero se complementan:

  EN_RXADDR = habilita el pipe para recibir datos
  EN_AA     = habilita el ACK automático para ese pipe

  Podés tener un pipe habilitado para recibir pero sin mandar ACK (EN_RXADDR=1, EN_AA=0),
  útil si el transmisor no espera confirmación.


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro SETUP_AW %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 Registro SETUP_AW (dirección 0x03)

  AW = Address Width — define el ancho de las direcciones de todos los pipes.

  Bit:    7      6      5      4      3      2      1      0
         reserv reserv reserv reserv reserv reserv  AW1   AW0

  Solo 2 bits, todos los demás son reservados.

  ---
  Valores posibles

  ┌─────┬─────┬───────┬───────────────────┐
  │ AW1 │ AW0 │ Valor │       Ancho       │
  ├─────┼─────┼───────┼───────────────────┤
  │ 0   │ 0   │ 0x00  │ Ilegal            │
  ├─────┼─────┼───────┼───────────────────┤
  │ 0   │ 1   │ 0x01  │ 3 bytes           │
  ├─────┼─────┼───────┼───────────────────┤
  │ 1   │ 0   │ 0x02  │ 4 bytes           │
  ├─────┼─────┼───────┼───────────────────┤
  │ 1   │ 1   │ 0x03  │ 5 bytes ← default │
  └─────┴─────┴───────┴───────────────────┘

  ---
  Consideraciones

  Aplica a todos los pipes a la vez — no podés tener P0 con 5 bytes y P1 con 3 bytes. Es un ajuste global.

  A mayor ancho, más robusto pero más overhead:
  5 bytes → 40 bits de dirección, menos colisiones, más bytes por paquete
  3 bytes → 24 bits de dirección, útil si querés minimizar el tamaño del paquete

  TX y RX deben coincidir — si el transmisor usa 5 bytes, el receptor también debe tener SETUP_AW=0x03,
   sino no se reconocen los paquetes.

  ---
  Configuracion en el proyecto:

  No aparece configurado explícitamente en nrfSetModeTX, por lo tanto queda en el valor default del nRF
  al encender: 0x03 → 5 bytes, que coincide con la dirección que usás:

  uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};  // 5 bytes

  0xE7E7E7E7E7 es la dirección default que trae el nRF24L01 en el registro TX_ADDR al encender, según el datasheet


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro SETUP_RETR  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Registro SETUP_RETR (dirección 0x04)

  RETR = Retransmission — configura cuánto tiempo esperar antes de reintentar y cuántas veces reintentar si no llega el ACK.

  Bit:    7      6      5      4      3      2      1      0
          ARD3  ARD2   ARD1   ARD0   ARC3   ARC2   ARC1   ARC0
          └─────────────────┘        └─────────────────┘
             ARD (delay)                  ARC (count)

  ---
  ARD — Auto Retransmit Delay (bits 7:4)

  Tiempo que espera el TX antes de reintentar, contado desde el inicio de la transmisión anterior:

  ┌─────┬───────────────────┐
  │ ARD │       Delay       │
  ├─────┼───────────────────┤
  │ 0x0 │ 250 µs            │
  ├─────┼───────────────────┤
  │ 0x1 │ 500 µs            │
  ├─────┼───────────────────┤
  │ 0x2 │ 750 µs            │
  ├─────┼───────────────────┤
  │ ... │ +250 µs cada paso │
  ├─────┼───────────────────┤
  │ 0xF │ 4000 µs           │
  └─────┴───────────────────┘

  El delay debe ser mayor que el tiempo que tarda el receptor en procesar y responder el ACK.
   Con payloads grandes o velocidad 250kbps hay que aumentarlo.

  ---
  ARC — Auto Retransmit Count (bits 3:0)

  Cantidad máxima de reintentos:

  ┌─────┬───────────────────────────────────────────────┐
  │ ARC │                  Reintentos                   │
  ├─────┼───────────────────────────────────────────────┤
  │ 0x0 │ 0 (sin reintentos, deshabilita retransmisión) │
  ├─────┼───────────────────────────────────────────────┤
  │ 0x1 │ 1                                             │
  ├─────┼───────────────────────────────────────────────┤
  │ ... │ ...                                           │
  ├─────┼───────────────────────────────────────────────┤
  │ 0xF │ 15 reintentos máximo                          │
  └─────┴───────────────────────────────────────────────┘

  Si se agotan todos los reintentos sin ACK → se activa el flag MAX_RT en STATUS.

  ---
  Valores típicos

  ┌───────┬────────┬─────┬────────────────────────────────────┐
  │ Valor │  ARD   │ ARC │                Uso                 │
  ├───────┼────────┼─────┼────────────────────────────────────┤
  │ 0x00  │ 250µs  │ 0   │ Tu proyecto — sin reintentos       │
  ├───────┼────────┼─────┼────────────────────────────────────┤
  │ 0x1A  │ 500µs  │ 10  │ Comunicación confiable estándar    │
  ├───────┼────────┼─────┼────────────────────────────────────┤
  │ 0xFF  │ 4000µs │ 15  │ Máxima tolerancia a interferencias │
  ├───────┼────────┼─────┼────────────────────────────────────┤
  │ 0x03  │ 250µs  │ 3   │ Balance velocidad/confiabilidad    │
  └───────┴────────┴─────┴────────────────────────────────────┘

  ---
  Primera versión del proyecto:

  nrfWriteReg(NRF_SETUP_RETR, 0x00);  // ARD=250µs, ARC=0

  ARC=0 significa que directamente no reintenta nunca. El ARD no importa porque sin reintentos nunca se usa.
  Va de la mano con EN_AA=0x00 — no tiene sentido reintentar si no hay ACK que confirme si llegó o no.




%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro RF_CH %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Registro RF_CH (dirección 0x05)

  RF Channel — define la frecuencia en la que opera el nRF24L01.

  Bit:    7      6      5      4      3      2      1      0
         reserv  RF6    RF5    RF4    RF3    RF2    RF1    RF0
                 └──────────────────────────────────────┘
                                canal (0-127)

  Solo 7 bits útiles, valor de 0 a 127.

  ---
  Cómo se calcula la frecuencia

  F = 2400 MHz + RF_CH MHz

  ┌───────┬─────────────────────────┐
  │ RF_CH │       Frecuencia        │
  ├───────┼─────────────────────────┤
  │ 0x00  │ 2.400 GHz               │
  ├───────┼─────────────────────────┤
  │ 0x01  │ 2.401 GHz               │
  ├───────┼─────────────────────────┤
  │ 0x4C  │ 2.476 GHz ← Seleccionado│
  ├───────┼─────────────────────────┤
  │ 0x7F  │ 2.527 GHz               │
  └───────┴─────────────────────────┘

  ---
  Consideraciones

  WiFi ocupa canales 2.412–2.484 GHz — se superpone con buena parte del rango del nRF. Los canales más limpios
  suelen ser los extremos:

  2.400 – 2.410 GHz  →  canales 0–10   (debajo del WiFi)
  2.484 – 2.527 GHz  →  canales 84–127 (arriba del WiFi)

  Bluetooth también opera en 2.4 GHz usando frequency hopping, puede interferir en cualquier canal.

  TX y RX deben estar en el mismo canal — obvio pero crítico. Si el transmisor está en 76 y el receptor en 77 no se escuchan.

  ---
  Primera versión del proyecto:

  nrfSetModeTX(addr, 76, 4);  // canal 76 → 2.476 GHz

  Canal 76 cae dentro del rango WiFi. Funciona para desarrollo pero en un entorno con mucho WiFi podría haber interferencias.
  Para producción convendría probar canales por encima de 84. REVISAR!!!

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro RF_SETUP  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Registro RF_SETUP (dirección 0x06)

  Bit:    7       	6      5      	4     3      2      	1      	0
         CONT_WAVE  —   RF_DR_LOW PLL_LOCK RF_DR_HIGH RF_PWR1 RF_PWR0  —

  Controla la velocidad de transmisión y la potencia de salida.

  ---
  RF_PWR — Potencia de salida (bits 2:1)

  ┌─────────┬─────────┬───────┬──────────────────────────────┐
  │ RF_PWR1 │ RF_PWR0 │ Valor │           Potencia           │
  ├─────────┼─────────┼───────┼──────────────────────────────┤
  │ 0       │ 0       │ 0x00  │ -18 dBm (mínima)             │
  ├─────────┼─────────┼───────┼──────────────────────────────┤
  │ 0       │ 1       │ 0x02  │ -12 dBm                      │
  ├─────────┼─────────┼───────┼──────────────────────────────┤
  │ 1       │ 0       │ 0x04  │ -6 dBm                       │
  ├─────────┼─────────┼───────┼──────────────────────────────┤
  │ 1       │ 1       │ 0x06  │ 0 dBm (máxima) ← tu proyecto │
  └─────────┴─────────┴───────┴──────────────────────────────┘

  Mayor potencia = mayor alcance pero mayor consumo.

  ---
  RF_DR — Data Rate (bits 5 y 3)

  La velocidad se define con dos bits separados:

  ┌──────────────────┬───────────────────┬──────────────────────┐
  │ RF_DR_LOW (bit5) │ RF_DR_HIGH (bit3) │      Velocidad       │
  ├──────────────────┼───────────────────┼──────────────────────┤
  │ 0                │ 0                 │ 1 Mbps ← Seleccionado│
  ├──────────────────┼───────────────────┼──────────────────────┤
  │ 0                │ 1                 │ 2 Mbps               │
  ├──────────────────┼───────────────────┼──────────────────────┤
  │ 1                │ 0                 │ 250 kbps             │
  ├──────────────────┼───────────────────┼──────────────────────┤
  │ 1                │ 1                 │ ilegal               │
  └──────────────────┴───────────────────┴──────────────────────┘


  Mayor velocidad  → menor alcance, menos robusto
  Menor velocidad  → mayor alcance, más robusto ante interferencias

  ---
  Otros bits

  PLL_LOCK (bit 4) — solo para testing, fuerza el PLL encendido. Siempre en 0 en uso normal.

  CONT_WAVE (bit 7) — emite una portadora continua sin datos, solo para pruebas de RF. Siempre en 0 en uso normal.

  ---
  En el proyecto:

  nrfWriteReg(NRF_RF_SETUP, 0x06);  // 0b00000110

  CONT_WAVE  = 0
  RF_DR_LOW  = 0  ┐
  RF_DR_HIGH = 0  ┘ → 1 Mbps
  PLL_LOCK   = 0
  RF_PWR     = 11 → 0 dBm (máxima potencia)

  Máxima potencia y velocidad media — buena combinación para desarrollo donde querés alcance sin preocuparte por consumo.

	Pendiente: Ver de bajar la potencia, o enviar la potencia con el transmisor.



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro STATUS %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Registro STATUS (dirección 0x07)

  Bit:    7      6      5      4      3    2    1    0
         reserv RX_DR TX_DS MAX_RT reserv RX_P_NO   TX_FULL
                                          └────────┘

  Es el registro de estado — refleja lo que está pasando en el nRF en tiempo real. Se puede leer en cualquier momento, incluso viene automáticamente como primer byte en cualquier
  transacción SPI.

  ---
  Bit 6 — RX_DR (RX Data Ready)

  1 → hay un paquete nuevo en el RX FIFO esperando ser leído
  0 → RX FIFO vacío
  Se limpia escribiendo 1 en este bit.

  ---
  Bit 5 — TX_DS (TX Data Sent)

  1 → paquete transmitido correctamente
      (si EN_AA=1, significa que además llegó el ACK)
  0 → no hubo transmisión reciente
  Se limpia escribiendo 1 en este bit.

  ---
  Bit 4 — MAX_RT (Maximum Retransmits)

  1 → se agotaron los reintentos sin recibir ACK
  0 → normal
  Mientras este bit esté en 1, el nRF no puede transmitir nada más. Hay que limpiarlo escribiendo 1, y además hacer FLUSH_TX.

  ---
  Bits 3:1 — RX_P_NO (RX Pipe Number)

  Indica por qué pipe llegó el paquete que está en el RX FIFO:


  ┌───────┬───────────────┐
  │ Valor │  Significado  │
  ├───────┼───────────────┤
  │ 000   │ Paquete en P0 │
  ├───────┼───────────────┤
  │ 001   │ Paquete en P1 │
  ├───────┼───────────────┤
  │ 010   │ Paquete en P2 │
  ├───────┼───────────────┤
  │ 011   │ Paquete en P3 │
  ├───────┼───────────────┤
  │ 100   │ Paquete en P4 │
  ├───────┼───────────────┤
  │ 101   │ Paquete en P5 │
  ├───────┼───────────────┤
  │ 110   │ No usado      │
  ├───────┼───────────────┤
  │ 111   │ RX FIFO vacío │
  └───────┴───────────────┘

  ---
  Bit 0 — TX_FULL

  1 → TX FIFO lleno (no se puede escribir más hasta que se transmita)
  0 → TX FIFO tiene espacio disponible

  ---
  Característica especial — STATUS en cada transacción SPI

  Cada vez que bajás CSN y mandás cualquier comando, el primer byte que devuelve el nRF por MISO es siempre el STATUS. Por eso en el driver tenés _rx_buf aunque solo quieras escribir —
   ese primer byte siempre es el STATUS sin pedirlo explícitamente.

  MOSI:  [ 0x21 | 0x00 ]   ← escribir EN_AA = 0x00
  MISO:  [ 0x0E |  —  ]   ← STATUS viene solo, gratis

    ---
  En el proyecto:

  Con EN_AA=0 y siendo TX puro, los bits relevantes son:

  - TX_DS → se activa cuando termina cada nrfTransmit()
  - RX_DR → nunca se activa
  - MAX_RT → nunca se activa
  - TX_FULL → podría activarse si transmitís más rápido de lo que el FIFO se vacía


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro RX_ADDR_P0 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro TX_ADDR %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% Registro RX_PW_P0 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%









*/
