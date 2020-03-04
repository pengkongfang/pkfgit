#include "key.h"
#include "ddl.h"
#include "lcd_drive.h"
#include "gpio.h"
#include "Key.h"
void Press_Scan(void);
Key_Struct_Typdef Key_Struct;

/**************************************************************
��������:	KEY����
����˵��:  
��������ں���:
���ı�ע:
**************************************************************/
void KEY_Time_Interrupt(void)																				//KEY???????
{
		if(Key_Struct.Key_FD_State)
		{
			Key_Struct.Time_FD_Press++;//����
		}
}
/**************************************************************
��������:	ɨ�谴��ֵ
����˵��:  
��������ں���:
���ı�ע:
**************************************************************/
void Press_Scan(void)
{
	Key_Struct.Key_PressNum=0;
	if(!ReadKey_Up){Key_Struct.Key_PressNum=1;}											
	else if(!ReadKey_Down){Key_Struct.Key_PressNum=2;}	
	else if(!ReadKey_M){Key_Struct.Key_PressNum=3;}
	else if(!ReadKey_1){Key_Struct.Key_PressNum=4;}
	else if(!ReadKey_2){Key_Struct.Key_PressNum=5;}
	else if(!ReadKey_3){Key_Struct.Key_PressNum=6;}
	else if(!ReadKey_4){Key_Struct.Key_PressNum=7;}
	else 
	{
		if(Key_Struct.Time_FD_Press>1&&Key_Struct.Time_FD_Press<6)
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



void Key_Deal(void)																				 //KEY�����ж϶̰�����
{
	Press_Scan();
	switch(Key_Struct.Key_Index)
	{
			case 0://���
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
			case 1://�����̰���
							if(Key_Struct.Key_PressNum==Key_Struct.Old_key)//
							{
								Key_Struct.Key_FD_State=1; 
								if(Key_Struct.Time_FD_Press>8&&Key_Struct.Key_LongValue)
								{
									Key_Struct.Time_FD_Press=8;
									Key_Struct.Flag_Key_Short=0;
									Key_Struct.Flag_Key_Long=Key_Struct.Old_key;   
								}
							}
		}
	
		if(Key_Struct.Flag_Key_Long)
		{
			Key_Struct.KeyOut_Value=Key_Struct.Flag_Key_Long;
			Key_Struct.Flag_Key_Long=0;
			if(Key_Struct.KeyOut_Value==3)
			{
				LCD_Control(WM_MLONG);
			}
			else if(Key_Struct.KeyOut_Value==1)
			{
				LCD_Control(WM_UPLONG);
			}
			else if(Key_Struct.KeyOut_Value==2)
			{
				LCD_Control(WM_DWONLONG);
			}
		}
	 	else 
		{
			Key_Struct.KeyOut_Value=Key_Struct.Flag_Key_Short;
			Key_Struct.Flag_Key_Short=0;
			if(Key_Struct.KeyOut_Value)
			{
				LCD_Control(Key_Struct.KeyOut_Value);
			}
		}
}	


void key_init(void)
{
	stc_gpio_cfg_t stcGpioCfg;
    
	///< ��GPIO����ʱ���ſ�
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
	
	///< �˿ڷ�������->����
	stcGpioCfg.enDir = GpioDirIn;
	///< �˿�������������->����������
	stcGpioCfg.enDrv = GpioDrvL;
	///< �˿�����������->��
	stcGpioCfg.enPu = GpioPuEnable;
	stcGpioCfg.enPd = GpioPdDisable;
	///< �˿ڿ�©�������->��©����ر�
	stcGpioCfg.enOD = GpioOdDisable;
	///< �˿�����/���ֵ�Ĵ������߿���ģʽ����->AHB
	stcGpioCfg.enCtrlMode = GpioAHB;
	///< GPIO IO USER KEY��ʼ��
	Gpio_Init(KEY_UP_PORT, KEY_UP_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_DOWN_PORT,KEY_DOWN_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_1_PORT,KEY_1_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_2_PORT,KEY_2_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_3_PORT,KEY_3_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_4_PORT,KEY_4_PIN, &stcGpioCfg); 
	Gpio_Init(KEY_M_PORT,KEY_M_PIN, &stcGpioCfg); 
}







