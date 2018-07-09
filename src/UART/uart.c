#include "uart.h"
#include "../Timers/timers.h"
#include <sam3xa/include/sam3x8e.h>
#include <stdio.h>
#include <memory.h>

//UART input buffer (printf handles output buffer)
static char rxBuffer[UART_RX_BUFFER_SIZE];
static uint32_t irxBuffer;

static int readFromBuffer(char * ptr, int len);

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
  //in system call heavy workloads it is important that we give UART higher priority than SVCall (UART is priority 0, the default).
  //if we do not then UART can become un responsive. TODO move this? but where? hm.....
  NVIC_SetPriority(SVCall_IRQn, 1);

  //setup buffer
  memset(rxBuffer,0,UART_RX_BUFFER_SIZE*sizeof(char));
  irxBuffer = 0;

  //uart control reg  (enable receive and transmit)
  REG_UART_CR |= UART_CR_RXEN | UART_CR_TXEN;
}

uint32_t getRxBufferLenUart(){
  return irxBuffer;
}

bool nCharactersReadyUart(uint32_t n){
  if(irxBuffer >= n){
    return true;
  }
  else{
    return false;
  }
}

uint32_t RxHasNewLine(){
  for(uint32_t i =0; i < irxBuffer; i++){
    if(rxBuffer[i] == '\n'){
      return i;
    }
  }
  return 0;
}

bool lineReadyUart(){
  NVIC_DisableIRQ(IRQ_UART_ID);
  if(RxHasNewLine()){
    NVIC_EnableIRQ(IRQ_UART_ID);
    return true;
  }
  else {
    NVIC_EnableIRQ(IRQ_UART_ID);
    return false;
  }
}

uint32_t readDirectUART(uint8_t * buff, uint32_t n){
  return readFromBuffer(buff, n);
}

void getNextLineUart(char * buff, uint32_t maxLen){
  uint32_t cpyLen = RxHasNewLine() + 1;
  if(cpyLen == 1){
    //no new line
    memset(buff,0,maxLen);
    return;
  }

  if(cpyLen < maxLen){
    readFromBuffer(buff,cpyLen);
    buff[cpyLen - 1] = '\0';//remove new line
    if(cpyLen - 2 >= 0 && buff[cpyLen - 2] == '\r'){
      //remove carage return if present
      buff[cpyLen - 2] = '\0';
    }
  }
  else{
    readFromBuffer(buff,maxLen);
  }
}


//trigered on UART_RXRDY interrupt
void UART_ISR(void){
  //save receive msg in to buffer
  *(rxBuffer + irxBuffer) = (char)UART->UART_RHR;
  irxBuffer ++;
}

static int readFromBuffer(char * ptr, int len){
  NVIC_DisableIRQ(IRQ_UART_ID);

  int cpyLen = len - irxBuffer;
  if(cpyLen >= 0) cpyLen = irxBuffer;
  else cpyLen = len;

  if(cpyLen == 0){
    NVIC_EnableIRQ(IRQ_UART_ID);
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

  NVIC_EnableIRQ(IRQ_UART_ID);
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
