#include "misc.h"
#include "user_sever.h"
#include "oledpage.h"
#include "lcd_drive.h"
#include "lcd12864.h"
#include "menu_frame.h"
#include "uart_sever.h"
#include "beep.h"
uint8_t Back_To;
uint8_t RunTo_Slect=1;
void Run_Frame(int data);


uint8_t Open_movie_time=64;
void Open_movie(int data)
{
		
	 switch (data)
    {
    case WM_CREATE:
					Lcd_Write_Time(REAL_T,1,1);//5s
					Lcd_Write_Time(READ_BK,1,4);//5s
        break;
		case WM_TIMEOUT:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Lcd_Write_Time(REAL_T,0,1);//5s
				if(Para_str.FirstPower==0XBB)Creat_Frame(Reseting_Frame);
				else Creat_Frame(Main_Frame);
        break;
		case WM_REALTIME:
			if(Open_movie_time>25)
			{
				Open_movie_time--;
				if(Open_movie_time==60)Uart_Start();
				if(Open_movie_time<=30)Uart_Send_ReadHight();
				clear_screen();
				GRAM_ShowLattice(22,Open_movie_time,88,16,(const uint8_t *)Page_LOGO_WIN ,1);
			}
			else
			{
				//Uart_Start();
				Lcd_Write_Time(REAL_T,0,1);//5s
			}
			break;
    }
}

void Standby_Frame(int data)
{
	switch (data)
  {
    case WM_CREATE:
			lcd_switch(0);
			break;
		case WM_1:
		case WM_2:
		case WM_3:
		case WM_4:
		case WM_M:
		case WM_UP:
		case WM_DOWN:
		case WM_DWONLONG:
		case WM_UPLONG:
			lcd_switch(1);
			Creat_Frame(Main_Frame);
			break;
	}
}

static char bufd[10]={0};
void Main_Frame(int data)
{
		float temphight;
		switch (data)
    {
    case WM_CREATE:
				Uart_Send_ReadHight();
			//clear_page_edge(100,30);
			  clear_screen();	
				temphight=(float)Para_read(HIGHT_NOW);
				temphight/=10;
				if(temphight<=99)
				{
					sprintf(bufd,"%.1f%s",(float)temphight,(Para_read(MODE_UNIT))?"in":"cm");
				}
				else
				{
					sprintf(bufd,"%d%s",(int)temphight,(Para_read(MODE_UNIT))?"in":"cm");
					GRAM_Clear(96,25,101,57);
				}
				GRAM_ShowString(78,8,bufd,FONT16_DEFAULT);
				
				GRAM_ShowLattice(10,10,64,14,(const uint8_t *)Page_LOGO ,1);
				if(Para_read(TIME_SETREMINDER))
				GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_REMIND_HOME ,0); 
				
				if((Para_str.hight_str.TopSetFlag)&&(!Para_str.hight_str.DownSetFlag))
					GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_LIMITTOP ,1);
				else if((!Para_str.hight_str.TopSetFlag)&&(Para_str.hight_str.DownSetFlag))
					GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_LIMITDWON ,1);
				else if(Para_str.hight_str.TopSetFlag&&Para_str.hight_str.DownSetFlag)
					GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_LIMIT_HOME ,0);

					
				
				if(Para_read(MODE_KNOCK))
					GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_KNOCK_HOME ,0);
				if(Para_read(MODE_WILLBLOCK))
					GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_WILLBACK_HOME ,1);
				else if(Para_read(MODE_BLOCK))//
					GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_BACK_HOME ,1);
				Lcd_Write_Time(READ_OFF,1,10);//5s
				Lcd_Write_Time(REAL_T,1,8);//5s
        break;
		case WM_REALTIME:
				Uart_Send_ReadHight();
				temphight=(float)Para_read(HIGHT_NOW);
				temphight/=10;
				if(temphight<=99)
				{
					sprintf(bufd,"%.1f%s",(float)temphight,(Para_read(MODE_UNIT))?"in":"cm");
				}
				else
				{
					sprintf(bufd,"%d%s",(int)temphight,(Para_read(MODE_UNIT))?"in":"cm");
					GRAM_Clear(96,25,101,57);
				}
				GRAM_ShowString(78,8,bufd,FONT16_DEFAULT);
			break;
		case WM_UP:
		case WM_DOWN:
		case WM_DWONLONG:
		case WM_UPLONG:
			Lcd_Write_Time(REAL_T,0,10);//5s
			Lcd_Write_Time(READ_OFF,0,0);//5s
			Creat_Frame(Run_Frame);
			break;
		case WM_1:
		case WM_2:
		case WM_3:
		case WM_4:
			RunTo_Slect=data-3;
			if(Para_read((enum PARA_ENUM)data)==0)break;//如果没设置不跳转
			Lcd_Write_Time(READ_OFF,0,0);//5s
			Creat_Frame(RunisSet_Frame);
			break;
		case WM_MLONG:
			Menu_Frame_Init();
			Lcd_Write_Time(READ_OFF,0,0);//5s
			Creat_Frame(Menu_Frame);
			break;
		case WM_M:
			Lcd_Write_Time(READ_OFF,0,0);//5s
			Creat_Frame(HightSave_Frame);
			break;
		case WM_OFF:
				//Beep_SetNum(1,2,0);
				Lcd_Write_Time(READ_OFF,0,0);//5s
				Creat_Frame(Standby_Frame);
        break;
    }
}



void dis_Hight(uint8_t x,uint8_t y,float temphight)
{
	temphight/=10;
	if(temphight<=99)
	{
		sprintf(bufd,"%.1f",(float)temphight);
	}
	else
	{
		sprintf(bufd,"%d",(int)temphight);
		GRAM_Clear(x,y,x+73,y+32);
	}
	GRAM_ShowString(x,y,bufd,FONT32_DEFAULT);
}

void RunisSet_Frame(int data)
{
		float temph;
		switch (data)
    {
    case WM_CREATE:
				//k=(MAX_HIGHT-MIN_HIGHT)/(StartY-EndY);
				clear_screen();
				Lcd_Write_Time(REAL_T,0,0);//5s
				Lcd_Write_Time(READ_BK,1,60);//5s
				//GRAM_ShowString(50,25,"80.1",FONT32_DEFAULT);
				switch(RunTo_Slect)
				{
					case 1:
						temph=(float)Para_read(HIGHT_SET1);
						dis_Hight(50,25,temph);
						GRAM_ShowString(80,8,"M1",FONT16_DEFAULT);
						break;
					case 2:
						temph=(float)Para_read(HIGHT_SET2);
						dis_Hight(50,25,temph);
						GRAM_ShowString(80,8,"M2",FONT16_DEFAULT);
						break;
					case 3:
						temph=(float)Para_read(HIGHT_SET3);
						dis_Hight(50,25,temph);
						GRAM_ShowString(80,8,"M3",FONT16_DEFAULT);
						break;
					case 4:
						temph=(float)Para_read(HIGHT_SET4);
						dis_Hight(50,25,temph);
						GRAM_ShowString(80,8,"M4",FONT16_DEFAULT);
						break;
				}
				if(temph>Para_read(HIGHT_TOP)||temph<Para_read(HIGHT_DWON))
				{
					Creat_Frame(SetOverError_Frame);
					return;
				}
				Uart_RunToSet(RunTo_Slect);
				GRAM_ShowLattice(10,20,32,32,(const uint8_t *)Page_RUN_GO ,1);
				Lcd_Write_Time(READ_BK,1,3);//5s
        break;
		case WM_TIMEOUT:
				//Beep_SetNum(1,2,0);
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
        break;
		case WM_ARRIVE:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
				break;
		case WM_M:
		case WM_1:
		case WM_2:
		case WM_3:
		case WM_4:
		case WM_UP: 
		case WM_DOWN:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
				break;
    }
}

static uint8_t num,tempnum;
void Change_Sit_Stand(void)
{
	if(Para_read(HIGHT_NOW)<=Para_read(HIGHT_DWON)){num=1;}
	else if(Para_read(HIGHT_NOW)>=Para_read(HIGHT_TOP)){num=2;}
	else 
	{
		if(Para_read(HIGHT_NOW)<((Para_str.mode_str.unit_mode)?354:900))num=3;
		else num=4;
	}
	if(num!=tempnum)
	{
		tempnum=num;
		switch(tempnum)
		{
			case 1:
				GRAM_ShowLattice(10,20,32,32,(const uint8_t *)Page_RUN_DOWN ,1);
				break;
			case 2:
				GRAM_ShowLattice(10,20,32,32,(const uint8_t *)Page_RUN_TOP ,1);
				break;
			case 3:
				GRAM_ShowLattice(10,20,32,32,(const uint8_t *)Page_RUN_SIT ,1);
				break;
			case 4:
				GRAM_ShowLattice(10,20,32,32,(const uint8_t *)Page_RUN_STAND ,1);
				break;
		}
	}
}
extern void Para_init(void);
void Run_Frame(int data)
{
		uint8_t temph;
		static float k;
		static uint16_t Max_Hight,Min_Hight;
		switch (data)
    {
    case WM_CREATE:
				Max_Hight=(Para_str.mode_str.unit_mode)?(1800*0.394):(1800);
				Min_Hight=(Para_str.mode_str.unit_mode)?(200*0.394):(200);
				k=(Max_Hight-Min_Hight)/(StartY-EndY);
				tempnum=0;
				clear_screen();	
				temph=EndY+(float)(Para_read(HIGHT_NOW)-Min_Hight)/k;
				GRAM_Clear(108,0,111,temph);
				GRAM_Fill(108,temph,111,EndY);
				dis_Hight(50,25,(float)Para_read(HIGHT_NOW));
				Change_Sit_Stand();
				Lcd_Write_Time(READ_BK,1,3);//5s
        break;
		case WM_M:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
				break;
		case WM_MLONG:
				Menu_Frame_Init();
				Lcd_Write_Time(READ_OFF,0,0);//5s
				Creat_Frame(Menu_Frame);
				break;
		case WM_UPLONG:
		case WM_UP:
				Lcd_Write_Time(READ_BK,1,3);//5s
				Lcd_Write_Time(REAL_T,1,4);//5s
				Uart_Send_Forward(0);
				dis_Hight(50,25,Para_read(HIGHT_NOW));
				temph=EndY+(float)(Para_read(HIGHT_NOW)-Min_Hight)/k;
				GRAM_Clear(108,0,111,temph);
				GRAM_Fill(108,temph,111,EndY);
				Change_Sit_Stand();
				break;
		case WM_DOWN:
		case WM_DWONLONG:
				Lcd_Write_Time(READ_BK,1,3);//5s
				Lcd_Write_Time(REAL_T,1,4);//5s
				Uart_Send_Forward(1);
				dis_Hight(50,25,Para_read(HIGHT_NOW));
				temph=EndY+(float)(Para_read(HIGHT_NOW)-Min_Hight)/k;
				GRAM_Clear(108,0,111,temph);
				GRAM_Fill(108,temph,111,EndY);
				Change_Sit_Stand();
				break;		
		case WM_1:
		case WM_2:
		case WM_3:
		case WM_4:
			RunTo_Slect=data-3;
			if(Para_read((enum PARA_ENUM)data)==0)break;//如果没设置不跳转
			Lcd_Write_Time(READ_OFF,0,0);//5s
			Creat_Frame(RunisSet_Frame);
			break;
		case WM_REALTIME:
				Uart_Send_ReadHight();
				dis_Hight(50,25,Para_read(HIGHT_NOW));
				temph=EndY+(float)(Para_read(HIGHT_NOW)-Min_Hight)/k;
				GRAM_Clear(108,0,111,temph);
				GRAM_Fill(108,temph,111,EndY);
				Change_Sit_Stand();
				break;
		case WM_TIMEOUT:
				Lcd_Write_Time(REAL_T,0,0);//5s
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
        break;
    }
}

void OK_Frame(int data)
{
		switch (data)
    {
    case WM_CREATE:
				clear_screen();	
				GRAM_ShowLattice(50,20,32,32,(const uint8_t *)Page_OK ,1);
				Lcd_Write_Time(READ_BK,1,1);//5s
        break;
		case WM_TIMEOUT:
				Lcd_Write_Time(READ_BK,0,0);//5s
				if(Back_To)
				Creat_Frame(Menu_Frame);
				else
				Creat_Frame(Main_Frame);	
        break;
    }
}

void ERROR_Frame(int data)
{
		switch (data)
    {
    case WM_CREATE:
				clear_screen();	
				GRAM_ShowLattice(50,20,32,32,(const uint8_t *)Page_ERROR ,1);
				Lcd_Write_Time(READ_BK,1,1);//5s
        break;
		case WM_TIMEOUT:
				Lcd_Write_Time(READ_BK,0,0);//5s
				if(Back_To)
				Creat_Frame(Menu_Frame);
				else
				Creat_Frame(Main_Frame);	
        break;
    }
}

void retry_Frame(int data)
{
		switch (data)
    {
    case WM_CREATE:
				clear_screen();	
				GRAM_ShowString(40,23,"请重试!",FONT16_DEFAULT);
				Lcd_Write_Time(READ_BK,1,1);//5s
        break;
		case WM_TIMEOUT:
				//Beep_SetNum(1,2,0);
				Lcd_Write_Time(READ_BK,0,0);//5s
				if(Back_To)
				Creat_Frame(Menu_Frame);
				else
				Creat_Frame(Main_Frame);	
        break;
    }
}

static uint8_t now_select;

void HightSavetWinkle_Frame(int data)
{
	static uint8_t Now_Pat,Now_Cont;
	switch (data)
	{
		case WM_CREATE:
			Now_Cont=0;
			Lcd_Write_Time(READ_BK,1,3);//5s
			Lcd_Write_Time(REAL_T,1,3);//5s
			break;
		case WM_REALTIME:
			Now_Pat=!Now_Pat;
			Now_Cont++;
			switch(now_select)
			{
				case 1:
					if(Now_Cont>=3)Para_write(HIGHT_SET1,Para_read(HIGHT_NOW),0);
					if(Now_Pat)
					{
						GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_NUM1 ,0);
						clear_page_edge(10,30); 
					}
					else
						GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_NUM1 ,1);
					break;
				case 2:
					if(Now_Cont>=3)Para_write(HIGHT_SET2,Para_read(HIGHT_NOW),0);
					if(Now_Pat)
					{
						GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_NUM2 ,0);
						clear_page_edge(40,30);
					}
					else
						GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_NUM2 ,1);
					break;
				case 3:
					if(Now_Cont>=3)Para_write(HIGHT_SET3,Para_read(HIGHT_NOW),0);
					if(Now_Pat)
					{
						GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_NUM3 ,0);
						clear_page_edge(70,30);
					}
					else
						GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_NUM3 ,1);
					break;
				case 4:
					if(Now_Cont==3)Para_write(HIGHT_SET4,Para_read(HIGHT_NOW),0);
					if(Now_Pat)
					{
						GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_NUM4 ,0);
						clear_page_edge(100,30);
					}
					else
						GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_NUM4 ,1);
					break;
				default:
					break;
			}
			break;
		case WM_TIMEOUT:
			Back_To=0;
			Creat_Frame(ERROR_Frame);
			break;
		case WM_ARRIVE:
				switch(now_select)
				{
					case 1:
						Para_write(HIGHT_SET1,Para_read(HIGHT_NOW),1);
						break;
					case 2:
						Para_write(HIGHT_SET2,Para_read(HIGHT_NOW),1);
						break;
					case 3:
						Para_write(HIGHT_SET3,Para_read(HIGHT_NOW),1);
						break;
					case 4:
						Para_write(HIGHT_SET4,Para_read(HIGHT_NOW),1);
					break;
				}		
				Back_To=0;
				Creat_Frame(OK_Frame);
				break;
	}
}
void HightSave_Frame(int data)
{
	float temphight;
		switch (data)
    {
			case WM_CREATE:
					clear_screen();	
					temphight=(float)Para_read(HIGHT_NOW);
					temphight/=10;
					if(temphight<=99)
					{
						sprintf(bufd,"%.1f%s",(float)temphight,(Para_read(MODE_UNIT))?"in":"cm");
					}
					else
					{
						sprintf(bufd,"%d%s",(int)temphight,(Para_read(MODE_UNIT))?"in":"cm");
						GRAM_Clear(98,25,101,57);
					}
					GRAM_ShowString(80,8,bufd,FONT16_DEFAULT);
					if(Para_read(HIGHT_SET1))
					{
						GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_NUM1 ,0);
						clear_page_edge(10,30); 
					}
					else
						GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_NUM1 ,1);
					if(Para_read(HIGHT_SET2))
					{
						GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_NUM2 ,0);
						clear_page_edge(40,30);
					}
					else
						GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_NUM2 ,1);
					if(Para_read(HIGHT_SET3))
					{
						GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_NUM3 ,0);
						clear_page_edge(70,30);
					}
					else
						GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_NUM3 ,1);
					if(Para_read(HIGHT_SET4))
					{
						GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_NUM4 ,0);
						clear_page_edge(100,30);
					}
					else
						GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_NUM4 ,1);
					Lcd_Write_Time(READ_BK,1,3);//5s
					break;
			case WM_TIMEOUT:
					//Beep_SetNum(1,2,0);
					Lcd_Write_Time(READ_BK,0,0);//5s
					Creat_Frame(Main_Frame);
					break;
			case WM_1:
				GRAM_ShowLattice(10,30,24,24,(const uint8_t *)Page_NUM1 ,0);
				clear_page_edge(10,30); 
				now_select=1;
				Creat_Frame(HightSavetWinkle_Frame);
				break;
			case WM_2:
				GRAM_ShowLattice(40,30,24,24,(const uint8_t *)Page_NUM2 ,0);
				clear_page_edge(40,30);
				now_select=2;
				Creat_Frame(HightSavetWinkle_Frame);
				break;
			case WM_3:
				GRAM_ShowLattice(70,30,24,24,(const uint8_t *)Page_NUM3 ,0);
				clear_page_edge(70,30);
				now_select=3;
				Creat_Frame(HightSavetWinkle_Frame);
				break;
			case WM_4:
				GRAM_ShowLattice(100,30,24,24,(const uint8_t *)Page_NUM4 ,0);
				clear_page_edge(100,30);
				now_select=4;
				Creat_Frame(HightSavetWinkle_Frame);
				break;
			case WM_M:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);
				break;
			case WM_MLONG:
				Menu_Frame_Init();
				Lcd_Write_Time(READ_OFF,0,0);//5s
				Creat_Frame(Menu_Frame);
				break;
    }
}

void SetOverError_Frame(int data)
{
		switch (data)
    {
			case WM_CREATE:
					clear_screen();	
					Lcd_Write_Time(READ_BK,0,0);//5s
					GRAM_ShowLattice(30,15,32,32,(const uint8_t *)Page_PROBLEM ,1);
					GRAM_ShowString(80,23,"超限",FONT16_DEFAULT);
					Lcd_Write_Time(READ_BK,1,1);//5s
					//full_display();
					break;
			case WM_1:
			case WM_2:
			case WM_3:
			case WM_4:
			case WM_M:
			case WM_UP:
			case WM_DOWN:
					Creat_Frame(Main_Frame);
        break;
			case WM_TIMEOUT:
				Lcd_Write_Time(READ_BK,0,0);//5s
				Creat_Frame(Main_Frame);	
        break;
    }
}


void Problem_Frame(int data)
{
		switch (data)
    {
			case WM_CREATE:
					clear_screen();	
					Lcd_Write_Time(READ_BK,0,0);//5s
					GRAM_ShowLattice(30,15,32,32,(const uint8_t *)Page_PROBLEM ,1);
					sprintf(bufd,"E%02X",Para_read(PROBLEM));
					GRAM_ShowString(80,23,bufd,FONT16_DEFAULT);
					break;
			case WM_1:
			case WM_2:
			case WM_3:
			case WM_4:
			case WM_M:
			case WM_UP:
			case WM_DOWN:
					//Beep_SetNum(1,2,0);
					Uart_CancelError();
					//Creat_Frame(Main_Frame);
					break;
			case WM_ARRIVE:
				if(Para_read(RELEASE)==2)
					Creat_Frame(Reseting_Frame);
				else
					Creat_Frame(Main_Frame);
				break;
    }
}


void Ring_Frame(int data)
{
	static uint8_t ringstate;
		switch (data)
    {
			case WM_CREATE:
					Beep_SetNum(40,2,2);
					clear_screen();	
					Lcd_Write_Time(READ_BK,1,60);//5s
					Lcd_Write_Time(REAL_T,1,1);//5s
					GRAM_ShowLattice(15,15,23,32,(const uint8_t *)Page_SIT,1);
					GRAM_ShowLattice(95,15,17,32,(const uint8_t *)Page_STAND,1);
					break;
			case WM_REALTIME:
					switch(ringstate)
					{
						case 0:
							GRAM_ShowString(45,15,">",FONT16_DEFAULT);
							Lcd_Write_Time(REAL_T,1,20);//2s
							ringstate++;
							break;
						case 1:
							GRAM_ShowString(65,15,">",FONT16_DEFAULT);
							Lcd_Write_Time(REAL_T,1,20);//1s
							ringstate++;
							break;
						case 2:
							GRAM_ShowString(85,15,">",FONT16_DEFAULT);
							Lcd_Write_Time(REAL_T,1,20);//1s
							ringstate++;
							break;
						case 3:
							GRAM_Clear(40,15,95,31);
							Lcd_Write_Time(REAL_T,1,20);//1s
							ringstate=0;
							break;
						default:
							break;
					}
					//Beep_SetNum(1,2,0);
					//Lcd_Write_Time(READ_BK,0,0);//5s
					break;
				case WM_1:
				case WM_2:
				case WM_3:
				case WM_4:
				case WM_M:
				case WM_UP:
				case WM_DOWN:
				case WM_TIMEOUT:
					Beep_SetNum(0,2,2);
					Lcd_Write_Time(READ_BK,0,0);//5s
					beep_write(0);
					ringstate=0;
					Creat_Frame(Main_Frame);
					break;
    }
}




