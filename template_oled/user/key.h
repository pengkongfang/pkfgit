/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/

#ifndef _KEY_H
#define _KEY_H

#include "ddl.h"
#include "stdbool.h"





#define KEY_UP_PORT    	 GpioPortA
#define KEY_UP_PIN    	 GpioPin12

#define KEY_DOWN_PORT    GpioPortB
#define KEY_DOWN_PIN     GpioPin0

#define KEY_1_PORT     	 GpioPortB
#define KEY_1_PIN     	 GpioPin2

#define KEY_2_PORT     	 GpioPortA
#define KEY_2_PIN     	 GpioPin9

#define KEY_3_PORT     	 GpioPortA
#define KEY_3_PIN     	 GpioPin10

#define KEY_4_PORT    	 GpioPortA
#define KEY_4_PIN     	 GpioPin11

#define KEY_M_PORT     	 GpioPortB
#define KEY_M_PIN     	 GpioPin1


#define ReadKey_Up       	Gpio_GetInputIO(KEY_UP_PORT, KEY_UP_PIN)
#define ReadKey_Down    	Gpio_GetInputIO(KEY_DOWN_PORT, KEY_DOWN_PIN)
#define ReadKey_1         Gpio_GetInputIO(KEY_1_PORT, KEY_1_PIN)
#define ReadKey_2       	Gpio_GetInputIO(KEY_2_PORT, KEY_2_PIN)
#define ReadKey_3        	Gpio_GetInputIO(KEY_3_PORT, KEY_3_PIN)
#define ReadKey_4        	Gpio_GetInputIO(KEY_4_PORT, KEY_4_PIN)
#define ReadKey_M       	Gpio_GetInputIO(KEY_M_PORT, KEY_M_PIN)



void KEY_Time_Interrupt(void);																				//KEY???????
void KEY_Init(void);																									//KEY初始化
void Key_Process(void);		
void Key_Deal(void);
//====按键标志位
typedef struct 
{
	uint8_t    Flag_Key_Short;
	uint8_t    Flag_Key_Long;
	uint16_t   Time_FD_Press;	
	uint8_t    Key_Index;//处理的步骤
	bool  Key_FD_State;
	uint8_t    Key_PressNum;
	bool    Key_LongValue;
	bool    Key_ShortValue;
	uint8_t      KeyOut_Value;
	uint8_t    Old_key;//付过来的按键值
}Key_Struct_Typdef;




void key_init(void);

#endif
