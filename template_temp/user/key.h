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





#define KEY_MODE_PORT    	 	3
#define KEY_MODE_PIN    	 	5
	
#define KEY_TEST_PORT    		0
#define KEY_TEST_PIN     		1

#define KEY_PWR_PORT     	 	0
#define KEY_PWR_PIN     	 	3




#define ReadKey_Mode       	Gpio_GetIO(KEY_MODE_PORT, KEY_MODE_PIN)
#define ReadKey_Test    		Gpio_GetIO(KEY_TEST_PORT, KEY_TEST_PIN)
#define ReadKey_Pwr         Gpio_GetIO(KEY_PWR_PORT, KEY_PWR_PIN)




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



void Press_Scan(void);
void key_init(void);

#endif
