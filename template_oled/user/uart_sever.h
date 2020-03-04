#ifndef _UART_SEVER_H
#define _UART_SEVER_H

#include "user_uart.h"
#include "stdbool.h"
#include "crc16.h"
#define UART_END 0X9D
#define UART_START 0X9B
#define UART_SERISE '\\'




#define  UART_SYNC        0x50
#define  UART_GETHIGHT    0x51
#define  UART_SET  	      0x52
#define  UART_IBALENCE    0x53
#define  UART_CBALENCE   	0x54
#define  UART_CBLOCK		 	0x55
#define  UART_RST       	0x56
#define  UART_MAIN       	0x57
#define  UART_RUNTOSET    0x58
#define  UART_HIGHTSET    0x59
#define  UART_RUN     	  0x5A
#define  RLEASE_PRBLEM    0x5B
#define  HAVEEND	     	  0x5C
void Uart_Send_ReadHight(void);
void Uart_Start(void);
void Uart_CancelError(void);
void Uart_Send_Forward(uint8_t Forward);
void Uart_RunToSet(uint8_t Num);
void Uart_Send_Reset(void);
void Uart_Send_CheckBlock(void);
void Uart_Send_CheckBalence(void);
void Uart_Send_AutoBalence(void);
void Uart_SetHight(uint8_t Num);
void make_uartSend_Data(uint8_t* data, uint8_t len);
void Uart_deal_message(uart_str *uart);
void Uart_parse(uart_str *uart);

#endif



