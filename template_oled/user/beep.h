#ifndef _BEEP_H
#define _BEEP_H

#include "ddl.h"
#include "stdbool.h"
#include "adt.h"
#include "lpm.h"
void beep_init(void);
void beep_write(bool sta);
void led_init(void);
void led_write(bool sta);

typedef struct 
{
	uint8_t   Beep_ONTime;
	uint8_t   Beep_Num;
	uint8_t   Beep_OFFTime;
	bool Beep_Delay_State;
	uint8_t Beep_Delay_Time;
	bool Beep_State;
}Beep_Struct_Typdef;
#define Beep_Open  Bt_M23_EnPWM_Output(TIM0, TRUE, FALSE);    //TIM0 端口输出使能
#define Beep_Close Bt_M23_EnPWM_Output(TIM0, FALSE, FALSE);    //TIM0 端口输出使能
void Beep_Deal(void);
void Beep_IO_Init(void);
void Beep_SetNum(uint8_t time,uint8_t ONTime,uint8_t OFFTime);
void Beep_Interrupt(void);


#endif


