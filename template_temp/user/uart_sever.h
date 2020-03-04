#ifndef _UART_SEVER_H
#define _UART_SEVER_H

#include "user_uart.h"
#include "stdbool.h"
#include "crc16.h"
#define UART_END 0X9D
#define UART_START 0X9B
#define UART_SERISE '\\'




#define  GetKeyValue        0x11
#define  DisplayLed         0x12
#define  MainBoardPowerOff  0x13
#define  TurnOnBuzzer       0x14
#define  TurnOffBuzzer      0x15
#define  SoftVersionDisplay 0x16
#define  DisSingleLED       0x17

enum{
	U_IDLE=0,
	U_LENTH=1,
	U_CMD=2,
	U_CRC=3,
	U_END=4,
}UART_STATE;

#endif



