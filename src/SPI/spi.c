#include "spi.h"
#include <sam3xa/include/sam3x8e.h>
#include <stdbool.h>

#define SPI_IRQ 24

static Pin cs;

#define SPI_MISO (0x1 << 25)//A25
#define SPI_MOSI (0x1 << 26)//A26
#define SPI_SCK  (0x1 << 27)//A27

void initSPI(Pin csPin){
  cs = csPin;

  REG_PMC_PCER0 |= PMC_PCER0_PID24; // enable SPI peripheral clock
  REG_PMC_PCER0 |= PMC_PCER0_PID11; // enable PIO controller A

  //disable pins so SPI can use them
  disablePin(PIO_BANK_A,SPI_MISO);
  disablePin(PIO_BANK_A,SPI_MOSI);
  disablePin(PIO_BANK_A,SPI_SCK);

  //disable SPI
  SPI0->SPI_CR = SPI_CR_SPIDIS;
  //reset SPI controller
  SPI0->SPI_CR = SPI_CR_SWRST;

  //set SPI mode (PIO mode A)
  PIOA->PIO_ABSR &= ~(SPI_MISO | SPI_MOSI | SPI_SCK);

  enablePin(cs.bank, cs.pin, PIO_OUT);
  SET_PIN(cs, 1);

  SPI0->SPI_MR = SPI_MR_MSTR | SPI_MR_PCS(0x00);
  SPI0->SPI_CSR[0] = SPI_CSR_SCBR(0xFF) | SPI_CSR_NCPHA;

  SPI0->SPI_CR = SPI_CR_SPIEN;
}

static void sendByte(uint8_t * sBuff, uint8_t * rBuff, int offset);
void sendSPI(uint8_t * sBuff, uint8_t * rBuff, uint16_t count){
  int i =0;
  for(i=0; i < count; i++){
    sendByte(sBuff, rBuff, i);
  }
}

static void sendByte(uint8_t * sBuff, uint8_t * rBuff, int offset){
  SPI0->SPI_TDR = (SPI0->SPI_TDR & 0xFFFFFF00) | *(sBuff + offset);

  while(!(SPI0->SPI_SR & SPI_SR_RDRF)){
    asm("");
  }

  if(rBuff != 0x0){
    *(rBuff + offset) = 0x000000FF & SPI0->SPI_RDR;
  }
  else {
    flushRcv();
  }
}

void flushRcv(){
  while(SPI0->SPI_SR & SPI_SR_RDRF){
    uint32_t garbage = SPI0->SPI_RDR;
  }
}

uint32_t getSPIStatus(){
  return SPI0->SPI_SR;
}

void setCS(uint8_t val){
  SET_PIN(cs, val);
}
