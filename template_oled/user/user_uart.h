#ifndef USER_UART_H_ 
#define USER_UART_H_ 

#include "uart.h"
#include "bt.h"
#include "lpm.h"
#include "gpio.h"
#include "dui.h"

typedef struct 
{
  unsigned char rxbuf[30];
	unsigned char datbuf[30];
	unsigned char rx_length;
	unsigned char rxdat_length;
	unsigned char wait_deal_num;
	unsigned char uart_want_get;
  ringBuffer_t uart_fifo;

}uart_str;
uint8_t uart_read(uint8_t* pData);
void uart_init(void);
void uart_write(uint8_t * buff , uint8_t length);
extern uart_str Uart_1;

#endif


