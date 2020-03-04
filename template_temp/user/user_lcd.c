#include "user_lcd.h"
#include "lcd_drive.h"
#include "lcd12864.h"
#include "oledpage.h"
#include "bm43.h"

extern bool user_mode;
void lcd_init(void)
{
	LCD12864_INIT(); 
	GRAM_ShowString(30,20,"Temp:",FONT16_DEFAULT);
	GRAM_ShowString(70,20,"XX.XX",FONT16_DEFAULT);
	GRAM_ShowString(128-16,20,"¡æ",FONT16_DEFAULT);
	GRAM_ShowLattice(0,2,24,54,Temp_Page,0);
	Dis_Mode(user_mode);
	full_display(); 
}


void Dis_work(void)
{
	
}

char buff[20];
void Dis_Temp(void)
{
	sprintf(buff,"%.2f",(user_mode)?Get_BM43_Temp():Get_BM43_Temp()+5.0);
	
	GRAM_Clear(70,20,128,36);
	GRAM_ShowString(128-16,20,"¡æ",FONT16_DEFAULT);
	GRAM_ShowString(70,20,buff,FONT16_DEFAULT);
	full_display(); 
}


void Dis_Mode(uint8_t mode)
{
	if(!mode)
	{
		GRAM_ShowLattice(98,36,30,30,Man_Page,0);
	}
	else
	{
		GRAM_ShowLattice(98,36,30,30,Wuti_Page,0);
	}
	full_display(); 
}

