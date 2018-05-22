#ifndef SPI_H_
#define SPI_H_
#include "../PIO/pio.h"
#include <stdint.h>

void initSPI(Pin csPin);

/**
  send count bytes from sBuff to the slave device, and fill rBuff with
  count response bytes. if rBuff NULL response ignored
*/
void sendSPI(uint8_t * sBuff, uint8_t * rBuff, uint16_t count);

/**
  flush receive buffer
*/
void flushRcv();

uint32_t getSPIStatus();

/**
  change chip select pin (CS) between high and low (active low)
*/
void setCS(uint8_t val);

#endif /*SPI_H_*/
