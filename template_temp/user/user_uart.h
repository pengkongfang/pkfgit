#ifndef USER_UART_H_ 
#define USER_UART_H_ 

#include "uart.h"
#include "bt.h"
#include "lpm.h"
#include "gpio.h"
#include "dui.h"

typedef struct 
{
  unsigned char *rxbuf;
  unsigned char *txbuf;
	unsigned char *datbuf;
	unsigned char rx_length;
	unsigned char rxdat_length;
  ringBuffer_t uart_fifo;
	void (*init)(void);
	uint8_t (*read)(uint8_t* pData);
	void (*write)(uint8_t * buff , uint8_t length);
}uart_str;

void user_uart_register(uart_str *uart);
extern uart_str Uart_0;

#endif


