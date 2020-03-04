#include "misc.h"
#include "oledpage.h"
void Main_Frame(int data);

void Standby_Frame(int data)
{
    switch (data)
    {
    case WM_CREATE:
			  clear_screen();
				GRAM_ShowString(58,6,"Loctek",FONT16_DEFAULT);
				SW_Back_Time(1,5);//5s
        break;
		case WM_TIMEOUT:
				//Beep_SetNum(1,2,0);
				SW_Back_Time(0,0);//5s
				Creat_Frame(Main_Frame);
        break;
    }
}


void Main_Frame(int data)
{
		switch (data)
    {
    case WM_CREATE:
			  clear_screen();	
				GRAM_ShowString(58,6,"Loctek",FONT6_REVERSE);
				GRAM_ShowLattice(0,0,16,16,(const uint8_t *)Page_Sedentary_Reminder ,0);
				SW_Back_Time(1,5);//5s
        break;
		case WM_TIMEOUT:
				//Beep_SetNum(1,2,0);
				SW_Back_Time(0,0);//5s
				//Creat_Frame(Main_Frame_OnDelta);
        break;
    }
}


