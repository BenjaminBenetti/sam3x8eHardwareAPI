/**
@file
@brief UART hardware interface. (not USART!)
*/

#ifndef UART_H_
#define UART_H_
#include <stdint.h>
#include <stdbool.h>

// UART vector is peripheral interrupt 8.
#define IRQ_UART_ID 8
#define UART_RX_BUFFER_SIZE 256

/**
initialize the UART system. Once complete, printf and scanf will be usable for
UART input / output.
*/
void initUART(void);

/**
  @return the number of bytes (characters) ready for reading from the rx buffer
*/
uint32_t getRxBufferLenUart();

/**
  @return true if there are n characters ready to be read
*/
bool nCharactersReadyUart(uint32_t n);

/**
  @return true if there is a entire input line ready to be read.
*/
bool lineReadyUart();

/**
  read directly from the uart rx buffer.
  @param buff buffer in which to store the read characters
  @param n the number of characters to read
  @return the number of characters actualy read.
*/
uint32_t readDirectUART(uint8_t * buff, uint32_t n);

/**
  read the next ready line from the uart buffer
  @param buff the buffer in to which to copy the line
  @param maxLen the max number of bytes to copy to buff
*/
void getNextLineUart(char * buff, uint32_t maxLen);

#endif
