#ifndef UART_H_
#define UART_H_

// UART vector is peripheral interrupt 8.
#define IRQ_UART_ID 8
#define UART_RX_BUFFER_SIZE 256

void initUART(void);

#endif
