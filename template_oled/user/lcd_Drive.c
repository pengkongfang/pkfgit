#include "Lcd_Drive.h"
#include "LCD12864.h"

struct WM_Obj WM_Frame;
LCDTimeOut_Srtuct_Typdef LCDTimeOut_Str;

extern const char Page_ARROW_UP[];
extern const char Page_ARROW_DOWN[];

/*******************************************************
函数名称：获取语言
函数作用：读取语言数据
入口参数：界面指针  语言数据
出口参数：void
修改备注：
********************************************************/
void GetLang(Focus_Srtuct_Typdef *Focus,const char * Source_aLang[])
{ 
				unsigned char i=0;
        for(i=0; i<Focus->Max_Focus_Num+1; i++)
        Focus->Focus_Data[i]=(char*)Source_aLang[i];
	
				//Focus->Focus_Data[i]=(char*)Source_aLang[Focus->Max_Focus_Num];
				
}

/*******************************************************
函数名称：设置焦点
函数作用：在指定位置画一条黑块
入口参数：界面指针  焦点行数
出口参数：void
修改备注：
********************************************************/
void Set_Focus(Focus_Srtuct_Typdef *Focus)
{
    if(Focus->Now_Focus==0)
		{
			GRAM_ShowLattice(10,50,16,6,(const uint8_t *)Page_ARROW_DOWN,1);
		}
		else if(Focus->Now_Focus==Focus->Max_Focus_Num)
		{
			GRAM_ShowLattice(10,10,16,6,(const uint8_t *)Page_ARROW_UP,1);
		}
		else
		{
			GRAM_ShowLattice(10,10,16,6,(const uint8_t *)Page_ARROW_UP,1);
			GRAM_ShowLattice(10,50,16,6,(const uint8_t *)Page_ARROW_DOWN,1);
		}
}

/*******************************************************
函数名称：焦点增加1
函数作用：选定下移
入口参数：界面指针 
出口参数：void
修改备注：
********************************************************/
void Increase_Focus(Focus_Srtuct_Typdef *Focus)
{
    if(Focus->Now_Focus<Focus->Max_Focus_Num)
    {
        Focus->Now_Focus++;
        ClearFrame();
        GRAM_ShowString(MenuX,MenuY,Focus->Focus_Data[Focus->Now_Focus],FONT16_DEFAULT);
				if(Focus->Now_Focus==Focus->Max_Focus_Num)
				{
					GRAM_ShowLattice(10,10,16,6,(const uint8_t *)Page_ARROW_UP,1);
				}
				else
				{
					GRAM_ShowLattice(10,10,16,6,(const uint8_t *)Page_ARROW_UP,1);
					GRAM_ShowLattice(10,50,16,6,(const uint8_t *)Page_ARROW_DOWN,1);
				}
    }
		else 
		{
			//Beep_SetNum(1,10,0);
		}
}
/*******************************************************
函数名称：焦点减少1
函数作用：选定上移
入口参数：界面指针 
出口参数：void
修改备注：
********************************************************/
void Decrease_Focus(Focus_Srtuct_Typdef *Focus)
{
    if(Focus->Now_Focus>0)
    {
        Focus->Now_Focus--;
        ClearFrame();
        GRAM_ShowString(MenuX,MenuY,Focus->Focus_Data[Focus->Now_Focus],FONT16_DEFAULT);
				if(Focus->Now_Focus==0)
				{
					//Focus->Now_Focus=12;
					GRAM_ShowLattice(10,50,16,6,(const uint8_t *)Page_ARROW_DOWN,1);
				}
				else
				{
					GRAM_ShowLattice(10,10,16,6,(const uint8_t *)Page_ARROW_UP,1);
					GRAM_ShowLattice(10,50,16,6,(const uint8_t *)Page_ARROW_DOWN,1);
				}
    }
		else
		{
			//Beep_SetNum(1,10,0);
		}
}


/*******************************************************
函数名称：创建窗体
函数作用：创建一个窗体
入口参数：窗体的函数指针
出口参数：void
修改备注：
********************************************************/
void Creat_Frame(WM_CALLBACK  Frame)
{
    WM_Frame.cb=Frame;
		WM_Frame.cb(WM_CREATE);
}



void Lcd_Write_Time(int mode,bool state,uint16_t Time)
{
	
	switch(mode)
	{
		case READ_BK:
			if(state)
			{
				LCDTimeOut_Str.Back_TimeOut_Flag=1;
			}
			else 
			{
				LCDTimeOut_Str.Back_TimeOut_Flag=0;
			}
			LCDTimeOut_Str.SetBac_Time=Time*20;
			LCDTimeOut_Str.Back_Time=0;
			break;
		case READ_OFF:
			if(state)
			{
				LCDTimeOut_Str.OFF_Time=0;
				LCDTimeOut_Str.OFF_TimeOut_Flag=1;
			}
			else
			{
				LCDTimeOut_Str.OFF_TimeOut_Flag=0;
			}
			LCDTimeOut_Str.OFF_Time_Set=Time*40;
			LCDTimeOut_Str.OFF_Time=0;
			break;
		case REAL_T:
			LCDTimeOut_Str.realtime=0;
			LCDTimeOut_Str.realtime_flag=state;
			LCDTimeOut_Str.realtime_Set=Time;
		break;
		default:
			break;
	}
}


int Lcd_Read_Time(int mode)
{
	
	switch(mode)
	{
		case READ_BK:
			if(LCDTimeOut_Str.Back_TimeOut_Flag==false)return 0;
			else if(LCDTimeOut_Str.Back_Time>=LCDTimeOut_Str.SetBac_Time&&LCDTimeOut_Str.Back_TimeOut_Flag==true)
			{
				LCDTimeOut_Str.Back_Time=0;
				LCDTimeOut_Str.Back_TimeOut_Flag=0;
				return 1;
			}
			else return 0;
		case READ_OFF:
			if(LCDTimeOut_Str.OFF_TimeOut_Flag==false)return 0;
			else if(LCDTimeOut_Str.OFF_Time>=LCDTimeOut_Str.OFF_Time_Set&&LCDTimeOut_Str.OFF_TimeOut_Flag==true)
			{
				LCDTimeOut_Str.OFF_Time=0;
				LCDTimeOut_Str.OFF_TimeOut_Flag=0;
				return 1;
			}
			else return 0;
		case REAL_T:
			if(LCDTimeOut_Str.realtime_flag==false)return 0;
			else if(LCDTimeOut_Str.realtime>=LCDTimeOut_Str.realtime_Set&&LCDTimeOut_Str.realtime_flag==true)
			{
				LCDTimeOut_Str.realtime=0;
				return 1;
			}
			else return 0;
		default:
			return -1;
	}
}

void LCD_Control(int msg)
{
	switch(msg)
	{
		case WM_CREATE:                  	
		case WM_UP:                   		
		case WM_DOWN:                  		
		case WM_M:                   			
		case WM_1:                   			
		case WM_2:                   			
		case WM_3:                			  
		case WM_4:                 			     
		case WM_MLONG:   
		case WM_ARRIVE:
			LCDTimeOut_Str.Back_Time=0;
			LCDTimeOut_Str.OFF_Time=0;
			WM_Frame.cb(msg);
			full_display();
		break;
		case WM_REALTIME:
		case WM_OFF:
		case WM_TIMEOUT:  
		case WM_DWONLONG: 
		case WM_UPLONG: 				
			WM_Frame.cb(msg);
			full_display();
		break;
		default:
				break;
		
	}
}

/*******************************************************
函数名称：
函数作用：Lcd中断
入口参数：void
出口参数：void
修改备注：
********************************************************/
void LCD_Interrupt(void)
{
	if(LCDTimeOut_Str.Back_TimeOut_Flag)LCDTimeOut_Str.Back_Time++;
	if(LCDTimeOut_Str.OFF_TimeOut_Flag)LCDTimeOut_Str.OFF_Time++;
	if(LCDTimeOut_Str.realtime_flag)LCDTimeOut_Str.realtime++; 
}






