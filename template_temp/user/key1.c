/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/



#include "key.h"
#include "ddl.h"

/*
* Copyright (c) 2006-2018, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2018-10-17     flybreak      the first version
*/



key_str Key;

struct my_button btn_m = {0}, btn_up = {0}, btn_down = {0},btn_4 = {0},btn_1 = {0},btn_2 = {0},btn_3 = {0};

static void beep_key_press_long(uint8_t port,uint8_t pin)
{
    switch ((port*16+pin))
    {
    case KEY_UP_PIN:
        break;
		case KEY_DOWN_PIN:
        break;
		case KEY_1_PIN:
        break;
		case KEY_2_PIN:
        break;
		case KEY_3_PIN:
        break;
		case KEY_4_PIN:
        break;
		case KEY_M_PIN:
        break;
    }
}

static void beep_key_press_short(uint8_t port,uint8_t pin)
{
    switch ((port*16+pin))
    {
    case KEY_UP_PIN:
        break;
		case KEY_DOWN_PIN:
        break;
		case KEY_1_PIN:
        break;
		case KEY_2_PIN:
        break;
		case KEY_3_PIN:
        break;
		case KEY_4_PIN:
        break;
		case KEY_M_PIN:
        break;
    }
}

void btn_cb(struct my_button *button)
{
    switch (button->event)
    {
    case BUTTON_EVENT_CLICK_UP:
        beep_key_press_short(button->GPIO,button->pin);
        break;
    case BUTTON_EVENT_HOLD_CYC:
        beep_key_press_long(button->GPIO,button->pin);
        break;
    default:
			break;
    }
}



void key_interrupt(void)
{
	Key.scan_timel++;
}


int key_init(void)
{
    btn_m.press_logic_level = KEY_PRESS_LEVEL;
	
    btn_m.hold_cyc_period = 100;
    btn_m.cb = (my_button_callback)btn_cb;
    
    btn_1=btn_2=btn_3=btn_up=btn_4 = btn_down = btn_m;
    
		btn_1.pin = 	KEY_1_PIN%16;
		btn_1.GPIO = 	KEY_1_PIN/16;
		btn_2.pin = 	KEY_2_PIN%16;
		btn_2.GPIO = 	KEY_2_PIN/16;
		btn_3.pin = 	KEY_3_PIN%16;
		btn_3.GPIO = 	KEY_3_PIN/16;
		btn_4.pin = 	KEY_4_PIN%16;
		btn_4.GPIO = 	KEY_4_PIN/16;
		btn_up.pin = 	KEY_UP_PIN%16;
		btn_up.GPIO = 	KEY_UP_PIN/16;
		btn_down.pin = 	KEY_DOWN_PIN%16;
		btn_down.GPIO = 	KEY_DOWN_PIN/16;
		btn_m.pin = 	KEY_M_PIN%16;
		btn_m.GPIO = 	KEY_M_PIN/16;

    my_button_register(&btn_1);
    my_button_register(&btn_up);
    my_button_register(&btn_2);
		my_button_register(&btn_3);
		my_button_register(&btn_m);
		my_button_register(&btn_4);
		my_button_register(&btn_down);
    return 0;
}

void key_scan(void)
{
	if(Key.scan_timel>=2)
	{
		Key.scan_timel=0;
		my_button_scan();
	}
}

void key_register(key_str *key)
{
	key->init=key_init;
	key->read=key_scan;
	key->intertupt_cb=key_interrupt;
	key->init();
	key->scan_timel=0;
}

