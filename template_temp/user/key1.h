/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/

#ifndef BEEP_KEY_H
#define BEEP_KEY_H

#include "button.h"


#define KEY_UP_PIN    	 51
#define KEY_DOWN_PIN     50
#define KEY_1_PIN     	 37
#define KEY_2_PIN     	 38
#define KEY_3_PIN     	 39
#define KEY_4_PIN     	 49
#define KEY_M_PIN     	 36
#define KEY_PRESS_LEVEL  0

typedef struct 
{
  unsigned char scan_timel;
	int (*init)(void);
	void (*read)(void);
	void (*intertupt_cb)(void);
}key_str;


void key_register(key_str *key);

int key_init(void);                     //按键初始化
extern  key_str Key;
#endif
