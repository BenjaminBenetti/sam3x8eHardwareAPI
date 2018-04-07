/**
@file
@brief UART hardware interface. (not USART!)
*/

#ifndef UART_H_
#define UART_H_

// UART vector is peripheral interrupt 8.
#define IRQ_UART_ID 8
#define UART_RX_BUFFER_SIZE 256

/**
initialize the UART system. Once complete, printf and scanf will be usable for
UART input / output.
*/
void initUART(void);

#endif
