#include "Lcd_Drive.h"

struct WM_Obj WM_Frame;
lcd_str Lcd;
bool MyLanguage;


/*******************************************************
函数名称：界面参数初始化
函数作用：初始化界面的焦点
入口参数：void
出口参数：void
修改备注：
********************************************************/
void Gui_Init(void)
{

}

/*******************************************************
函数名称：获取语言
函数作用：读取语言数据
入口参数：界面指针  语言数据
出口参数：void
修改备注：
********************************************************/
void GetLang(Focus_Srtuct_Typdef *Focus,const char * Source_aLang[][2])
{
				unsigned char i=0;
        for(i=0; i<Focus->Max_Focus_Num; i++)
        Focus->Focus_Data[i]=(char*)Source_aLang[i][MyLanguage];
	
				Focus->Focus_Data[i]=(char*)Source_aLang[Focus->Max_Focus_Num][MyLanguage];
				
}

/*******************************************************
函数名称：
函数作用：在绘制区域内显示数字
入口参数：X Y 显示的数据  模式（反显或者正显）
出口参数：void
修改备注：
********************************************************/
void DisNum_InFill(unsigned int x, unsigned int y ,unsigned char num,bool Mode)
{
	char RBuff[8]={0};
	sprintf(RBuff,"%d",num);
	if(Mode)
	{
		GRAM_Fill(x,y,x+13,y+10);
	  GRAM_ShowString(x+1,y+2,RBuff,FONT6_REVERSE);
	}
	else
	{
		GRAM_Clear(x,y,x+13,y+10);
		GRAM_ShowString(x+1,y+2,RBuff,FONT6_DEFAULT);
	}
}
/*******************************************************
函数名称：
函数作用：在绘制区域内显示字符串
入口参数：X Y 显示的数据（反显）
出口参数：void
修改备注：
********************************************************/
void DisChar_InFill(unsigned int x, unsigned int y ,char *Buff)
{

	GRAM_Fill(x,y,x+20,y+10);
	GRAM_ShowString(x+1,y+2,Buff,FONT6_REVERSE);

}





