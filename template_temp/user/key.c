#include "ddl.h"
#include "lcd_drive.h"
#include "gpio.h"
#include "Key.h"
#include "user_lcd.h"
void Press_Scan(void);
Key_Struct_Typdef Key_Struct;


bool user_mode;

/**************************************************************
函数名称:	KEY防抖
函数说明:  
函数出入口函数:
更改备注:
**************************************************************/
void KEY_Time_Interrupt(void)																				//KEY???????
{
		if(Key_Struct.Key_FD_State)
		{
			Key_Struct.Time_FD_Press++;//防抖
		}
}
/**************************************************************
函数名称:	扫描按键值
函数说明:  
函数出入口函数:
更改备注:
**************************************************************/
void Press_Scan(void)
{
	Key_Struct.Key_PressNum=0;
	if(!ReadKey_Mode){Key_Struct.Key_PressNum=1;}											
	else if(!ReadKey_Test){Key_Struct.Key_PressNum=2;}	
	else if(!ReadKey_Pwr){Key_Struct.Key_PressNum=3;}
	else  
	{
		if(Key_Struct.Time_FD_Press>3&&Key_Struct.Time_FD_Press<=20)
		{
			Key_Struct.Time_FD_Press=0;
			Key_Struct.Flag_Key_Short=Key_Struct.Old_key;                              
			Key_Struct.Key_ShortValue=false;
		}  
		Key_Struct.Key_FD_State=0;
		Key_Struct.Flag_Key_Long=0;
		Key_Struct.Time_FD_Press=0;
		Key_Struct.Key_Index=0;
		Key_Struct.Key_ShortValue=true;
		Key_Struct.Key_LongValue=true;
	}
}



void Key_Deal(void)																				 //KEY处理判断短按长按
{
	Press_Scan();
	switch(Key_Struct.Key_Index)
	{
			case 0://入口
								if(Key_Struct.Key_PressNum)
								{
										Key_Struct.Old_key=Key_Struct.Key_PressNum;
										Key_Struct.Key_Index=1;
										Key_Struct.Time_FD_Press=0;
								}
								else
								{
										Key_Struct.Key_Index=0;
										Key_Struct.Time_FD_Press=0;
										Key_Struct.Key_FD_State=0;
								}
							break;
			case 1://处理长短按键
							if(Key_Struct.Key_PressNum==Key_Struct.Old_key)//
							{
								Key_Struct.Key_FD_State=1; 
								if(Key_Struct.Time_FD_Press>20&&Key_Struct.Key_LongValue)
								{
									Key_Struct.Time_FD_Press=21;
									Key_Struct.Flag_Key_Short=0;
									Key_Struct.Key_LongValue=false;
									Key_Struct.Flag_Key_Long=Key_Struct.Old_key;   
								}
							}
		}
	
		if(Key_Struct.Flag_Key_Long)
		{
			Key_Struct.KeyOut_Value=Key_Struct.Flag_Key_Long;
			if(Key_Struct.KeyOut_Value==1)
			{
				Dis_Temp();
			}
			Key_Struct.Flag_Key_Long=0;
		}
	 	else 
		{
			Key_Struct.KeyOut_Value=Key_Struct.Flag_Key_Short;
			Key_Struct.Flag_Key_Short=0;
			if(Key_Struct.KeyOut_Value)
			{
				switch(Key_Struct.KeyOut_Value)
				{
					case 3:
						Gpio_SetIO(2,3,0);
						break;
					case 2:
						user_mode=!user_mode;
						Dis_Mode(user_mode);
						break;
				}
			}
		}
}	


void key_init(void)
{
	Gpio_InitIOExt(KEY_MODE_PORT, KEY_MODE_PIN, GpioDirIn, TRUE, FALSE, FALSE, FALSE);
	Gpio_InitIOExt(KEY_TEST_PORT, KEY_TEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, FALSE);
	Gpio_InitIOExt(KEY_PWR_PORT, KEY_PWR_PIN, GpioDirIn, TRUE, FALSE, FALSE, FALSE);
}







