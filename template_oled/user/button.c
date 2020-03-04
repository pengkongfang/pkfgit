/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 */

#include <ddl.h>
#include "button.h"

/****    Debug     ****/
#define DBG_ENABLE
#define DBG_SECTION_NAME "button"
#define DBG_LEVEL        DBG_INFO
#define DBG_COLOR
#include <ddl.h>
#include <time.h>
#define MY_BUTTON_CALL(func, argv) \
    do { if ((func) != NULL) func argv; } while (0)

struct my_button_manage
{
    uint8_t num;
    struct tm timer;
    struct my_button *button_list[MY_BUTTON_LIST_MAX];
};

static struct my_button_manage button_manage;

int my_button_register(struct my_button *button)
{
    if (button->press_logic_level == 0)
    {
				Gpio_InitIOExt(button->GPIO,button->pin, GpioDirIn, TRUE, FALSE, FALSE, TRUE);
    }
    else
    {
       Gpio_InitIOExt(button->GPIO,button->pin, GpioDirIn, FALSE, TRUE, FALSE, TRUE);
    }
    
    button->cnt = 0;
    button->event = BUTTON_EVENT_NONE;
    button_manage.button_list[button_manage.num++] = button;
    
    return 0;
}

void my_button_scan(void)
{
    uint8_t i;
    uint16_t cnt_old=0;

    for (i = 0; i < button_manage.num; i++)
    {
        cnt_old = button_manage.button_list[i]->cnt;

        if (Gpio_GetIO(button_manage.button_list[i]->GPIO,button_manage.button_list[i]->pin) == button_manage.button_list[i]->press_logic_level)
        {
            button_manage.button_list[i]->cnt ++;

            if ((button_manage.button_list[i]->cnt >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS)&&(button_manage.button_list[i]->cnt <MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS)) /* BUTTON_DOWN */
            {
                //LOG_D("BUTTON_DOWN");
                button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_DOWN;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
            else if (button_manage.button_list[i]->cnt == MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_HOLD */
            {
               // LOG_D("BUTTON_HOLD");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
            else if (button_manage.button_list[i]->cnt > MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_HOLD_CYC */
            {
               // LOG_D("BUTTON_HOLD_CYC");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_CYC;
                if (button_manage.button_list[i]->hold_cyc_period && button_manage.button_list[i]->cnt % (button_manage.button_list[i]->hold_cyc_period / MY_BUTTON_SCAN_SPACE_MS) == 0)
                    MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
        }
        else
        {
            button_manage.button_list[i]->cnt = 0;
            if (cnt_old >= MY_BUTTON_DOWN_MS / MY_BUTTON_SCAN_SPACE_MS && cnt_old < MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_CLICK_UP */
            {
                //LOG_D("BUTTON_CLICK_UP");
                button_manage.button_list[i]->event = BUTTON_EVENT_CLICK_UP;
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
            else if (cnt_old >= MY_BUTTON_HOLD_MS / MY_BUTTON_SCAN_SPACE_MS) /* BUTTON_HOLD_UP */
            {
                //LOG_D("BUTTON_HOLD_UP");
                button_manage.button_list[i]->event = BUTTON_EVENT_HOLD_UP; 
                MY_BUTTON_CALL(button_manage.button_list[i]->cb, (button_manage.button_list[i]));
            }
        }
    }
}

