#include "uart.h"
#include "../Timers/timers.h"
#include <sam3xa/include/sam3x8e.h>
#include <stdio.h>
#include <memory.h>

//UART input buffer (printf handles output buffer)
static char rxBuffer[UART_RX_BUFFER_SIZE];
static uint32_t irxBuffer;

/*enable uart chip & channel 0 (for debug communcation).*/
void initUART(void){
  //set PIO Lines to UART


  REG_PMC_PCER0 |= PMC_PCER0_PID8; // enable UART peripheral clock
  REG_PMC_PCER0 |= PMC_PCER0_PID11; // enable PIO controller A

  //disable PIO lines (so UART can use them)
  REG_PIOA_PDR |= PIO_PA8 | PIO_PA9;
  //disable PIO interrupts on serial lines
  REG_PIOA_IDR |= PIO_PA8 | PIO_PA9;
  //enable pull up on UART pins
  REG_PIOA_PUER |= PIO_PA8 | PIO_PA9;
  //set to UART mode
  REG_PIOA_ABSR &= ~PIO_PA8 & ~PIO_PA9;

  //disable DMA
  REG_UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

  //restet UART
  REG_UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS | UART_CR_RSTSTA;

  //set baud rate to about 9600
  REG_UART_BRGR = UART_BRGR_CD(546);

  //disable parity
  REG_UART_MR = 0x0;
  REG_UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;

  //setup interrupts
  UART->UART_IDR = 0xFFFFFFFF;
  UART->UART_IER = UART_IER_RXRDY;
  NVIC_EnableIRQ(IRQ_UART_ID);

  //setup buffer
  memset(rxBuffer,0,UART_RX_BUFFER_SIZE*sizeof(char));
  irxBuffer = 0;

  //uart control reg  (enable receive and transmit)
  REG_UART_CR |= UART_CR_RXEN | UART_CR_TXEN;
}

//trigered on UART_RXRDY interrupt
void UART_ISR(void){
  //save receive msg in to buffer
  *(rxBuffer + irxBuffer) = (char)UART->UART_RHR;
  irxBuffer ++;
}

static int readFromBuffer(char * ptr, int len){
  int cpyLen = len - irxBuffer;
  if(cpyLen >= 0) cpyLen = irxBuffer;
  else cpyLen = len;

  if(cpyLen == 0){
    return 0;
  }
  else {
    memcpy(ptr,rxBuffer,cpyLen);
  }

  //shuffle buffer down
  if(cpyLen != irxBuffer){
    memcpy(rxBuffer,rxBuffer + cpyLen,(irxBuffer - cpyLen));
    irxBuffer = irxBuffer - cpyLen;
  }
  else {
    irxBuffer = 0;
  }

  return cpyLen;
}

/*
  override C std lib _write and _read methods
*/
int _write(int file, char *ptr, int len){
  for(uint32_t i=0; i < len;){
    if(UART->UART_SR & UART_SR_TXRDY){
      //transmit one character
      REG_UART_THR = *(ptr + i);
      i++;
    }
  }
  return len;
}

int _read (int fd, char *ptr, int len){
  //read as much as possbile and no less than 1 character (if we read zero scanf will think the "file" is closed)
  int readCount = 0;
  while(readCount == 0){
    readCount = readFromBuffer(ptr,len);
  }
  return readCount;
}
