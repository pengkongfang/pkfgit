#include "user_sever.h"
#include "beep.h"
#include "key.h"
#include "LCD_Drive.h"
#include "LCD12864.h"
#include "misc.h"
#include "timer.h"
#include "user_flash.h"
#include "menu_frame.h"
#include "uart_sever.h"

void  Para_init(void);
Para_Str_typedef Para_str;

void user_init(void)
{
		led_init();
		//Para_init();
		delay1ms(2000);
		timer2_init();
		uart_init();
		beep_init(); 
		Beep_SetNum(3,2,2);
		key_init();
		LCD12864_INIT();
		Creat_Frame(Open_movie);
		full_display();
		//Uart_Start();	
} 

void user_work(void)
{
	if(Lcd_Read_Time(REAL_T)==true)
	{
		LCD_Control(WM_REALTIME);
	}
	if(Lcd_Read_Time(READ_BK)==true)
	{
		LCD_Control(WM_TIMEOUT);
	}
	if(Lcd_Read_Time(READ_OFF)==true)
	{
		LCD_Control(WM_OFF);
	}
	Key_Deal();
	Beep_Deal();
	Uart_parse(&Uart_1);
}




void  Para_init(void)
{
	Para_str.hight_str.now_hight=200;
	Para_str.hight_str.down_hight=200;
	Para_str.hight_str.top_hight=1500;
	Para_str.hight_str.set_hight[0]=50;
	Para_str.hight_str.set_hight[1]=50;
	Para_str.hight_str.set_hight[2]=50;
	Para_str.hight_str.set_hight[3]=50;
	Para_str.time_str.set_Remindtime=15;
	Para_str.mode_str.Block_mode=1;
	Para_str.mode_str.Remind_mode=1;
	Para_str.mode_str.Knock_mode=0;
	Para_str.mode_str.unit_mode=0;
	Para_read(HIGHT_NOW);
}

void Para_write(enum PARA_ENUM dat ,uint32_t value,uint8_t istrue)
{
	uint8_t Sevd[10]={UART_SET,Para_str.time_str.set_Remindtime,Para_str.mode_str.Remind_mode,Para_str.mode_str.Block_mode,Para_str.mode_str.Knock_mode,Para_str.mode_str.unit_mode,\
										Para_str.hight_str.top_hight>>8&0xff,Para_str.hight_str.top_hight&0xff,Para_str.hight_str.down_hight>>8&0xff,Para_str.hight_str.down_hight&0xff};
	uint8_t Sevr[4]={UART_HIGHTSET,0};
	switch((uint8_t)dat)
	{
		case HIGHT_NOW: Para_str.hight_str.now_hight=value;break;
		case HIGHT_TOP : 
				if(MAX_HIGHT==value)Para_str.hight_str.TopSetFlag=false;
				else Para_str.hight_str.TopSetFlag=true;
				value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
				if(istrue)Para_str.hight_str.top_hight=value;
				else {Uart_1.uart_want_get=UART_SET;Sevd[6]=value>>8&0xff;Sevd[7]=value&0xff;make_uartSend_Data(Sevd,11);}
				break;
		case HIGHT_DWON: 
				if(MIN_HIGHT==value)Para_str.hight_str.DownSetFlag=false;
				else Para_str.hight_str.DownSetFlag=true;
				value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
				if(istrue)Para_str.hight_str.down_hight=value;
				else {Uart_1.uart_want_get=UART_SET;Sevd[8]=value>>8&0xff;Sevd[9]=value&0xff;make_uartSend_Data(Sevd,11);}
				break;
		case HIGHT_SET1:
			Uart_1.uart_want_get=UART_HIGHTSET;	
			value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
			if(istrue)Para_str.hight_str.set_hight[0]=value;
			else
			{
				Sevr[1]=1;
				Sevr[2]=value>>8&0xff;
				Sevr[3]=value&0xff;
				make_uartSend_Data(Sevr,4);
			}
			break;
		case HIGHT_SET2: 
			Uart_1.uart_want_get=UART_HIGHTSET;	
			value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
			if(istrue)Para_str.hight_str.set_hight[1]=value;
			else
			{
				Sevr[1]=2;
				Sevr[2]=value>>8&0xff;
				Sevr[3]=value&0xff;
				make_uartSend_Data(Sevr,4);
			}
			break;
		case HIGHT_SET3: 
			Uart_1.uart_want_get=UART_HIGHTSET;	
			value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
			if(istrue)Para_str.hight_str.set_hight[2]=value;
			else
			{
				Sevr[1]=3;
				Sevr[2]=value>>8&0xff;
				Sevr[3]=value&0xff;
				make_uartSend_Data(Sevr,4);
			}
			break;
		case HIGHT_SET4 : 
			Uart_1.uart_want_get=UART_HIGHTSET;	
			value=(Para_str.mode_str.unit_mode)?(value*2.54):(value);
			if(istrue)Para_str.hight_str.set_hight[3]=value;
			else
			{
				Sevr[1]=4;
				Sevr[2]=value>>8&0xff;
				Sevr[3]=value&0xff;
				make_uartSend_Data(Sevr,4);
			}
			break;
		case HIGHT_LIE: Para_str.lie=(value)?true:false;
										Para_str.hight_str.lie_hight=value;
		case TIME_SETREMINDER :if(istrue)Para_str.time_str.set_Remindtime=value;else { Uart_1.uart_want_get=UART_SET;Sevd[1]=value;make_uartSend_Data(Sevd,10);}break;
		case MODE_REMINDER: if(istrue)Para_str.mode_str.Remind_mode=value;else {Uart_1.uart_want_get=UART_SET;Sevd[2]=value;make_uartSend_Data(Sevd,10);}break;
		case MODE_WILLBLOCK:if(istrue) Para_str.mode_str.WillBlock_mode=value;else {Uart_1.uart_want_get=UART_SET;Sevd[3]|=value;make_uartSend_Data(Sevd,10);}break;		
		case MODE_BLOCK:if(istrue) Para_str.mode_str.Block_mode=value;else {Uart_1.uart_want_get=UART_SET;Sevd[3]=value;make_uartSend_Data(Sevd,10);}break;
		case MODE_KNOCK:if(istrue)Para_str.mode_str.Knock_mode=value; else {Uart_1.uart_want_get=UART_SET;Sevd[4]=value;make_uartSend_Data(Sevd,10);}break;
		case MODE_UNIT: if(istrue)Para_str.mode_str.unit_mode=value;else {Uart_1.uart_want_get=UART_SET;Sevd[5]=value;make_uartSend_Data(Sevd,10);}break;
		case MODE_VER: Para_str.version=value;;break;
	}
}

uint32_t Para_read(enum PARA_ENUM dat)
{
	switch((uint8_t)dat)
	{
		case HIGHT_NOW: return (Para_str.mode_str.unit_mode)?((Para_str.lie)?Para_str.hight_str.lie_hight*0.394:Para_str.hight_str.now_hight*0.394):((Para_str.lie)?Para_str.hight_str.lie_hight:Para_str.hight_str.now_hight);
		case HIGHT_TOP : return (Para_str.mode_str.unit_mode)?(Para_str.hight_str.top_hight*0.394):(Para_str.hight_str.top_hight);
		case HIGHT_DWON : return (Para_str.mode_str.unit_mode)?(Para_str.hight_str.down_hight*0.394):(Para_str.hight_str.down_hight);
		case HIGHT_SET1 : return (Para_str.mode_str.unit_mode)?(Para_str.hight_str.set_hight[0]*0.394):(Para_str.hight_str.set_hight[0]);
		case HIGHT_SET2 : return (Para_str.mode_str.unit_mode)?(Para_str.hight_str.set_hight[1]*0.394):(Para_str.hight_str.set_hight[1]);
		case HIGHT_SET3 :return  (Para_str.mode_str.unit_mode)?(Para_str.hight_str.set_hight[2]*0.394):(Para_str.hight_str.set_hight[2]);
		case HIGHT_SET4 : return (Para_str.mode_str.unit_mode)?(Para_str.hight_str.set_hight[3]*0.394):(Para_str.hight_str.set_hight[3]);
		case TIME_SETREMINDER : return Para_str.time_str.set_Remindtime;
		case MODE_REMINDER: return Para_str.mode_str.Remind_mode;
		case MODE_WILLBLOCK: return Para_str.mode_str.WillBlock_mode;
		case MODE_BLOCK: return Para_str.mode_str.Block_mode;
		case MODE_KNOCK:return Para_str.mode_str.Knock_mode;
		case MODE_UNIT:return  Para_str.mode_str.unit_mode;
		case MODE_VER:return  Para_str.version;
		case ACTIVE_SULT:return  Para_str.Set_Result;
		case PROBLEM:return  Para_str.problem;
		case RELEASE:return  Para_str.Relaseend;
		default:return 0;
	}
}




