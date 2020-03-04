#include "lcd12864.h"
#include "oledfont.h"
#include "beep.h"

unsigned char View_Memory[8][128]= {0};

#define CHINESE_ENABLE 1

/**
 ******************************************************************************
 ** \brief  ��ʼ���ⲿGPIO����
 **
 ** \return ��
 ******************************************************************************/
static void App_GpioInit(void)
{
    stc_gpio_cfg_t GpioInitStruct;
    DDL_ZERO_STRUCT(GpioInitStruct);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);

    //SPI0��������:����
    GpioInitStruct.enDrv = GpioDrvH;
    GpioInitStruct.enDir = GpioDirOut;

    Gpio_Init(GpioPortA, GpioPin7,&GpioInitStruct);
    Gpio_SetAfMode(GpioPortA, GpioPin7,GpioAf1);        //��������PA7��ΪSPI0_MOSI


    Gpio_Init(GpioPortA, GpioPin5,&GpioInitStruct);
    Gpio_SetAfMode(GpioPortA, GpioPin5,GpioAf1);         //��������PA05��ΪSPI0_SCK
}

/**
 ******************************************************************************
 ** \brief  ��ʼ��SPI
 **
 ** \return ��
 ******************************************************************************/
static void App_SPIInit(void)
{
    stc_spi_cfg_t  SpiInitStruct;

    Sysctrl_SetPeripheralGate(SysctrlPeripheralSpi0,TRUE);

    //SPI0ģ�����ã�����
    SpiInitStruct.enSpiMode = SpiMskMaster;   //����λ����ģʽ
    SpiInitStruct.enPclkDiv = SpiClkMskDiv2;  //�����ʣ�fsys/128
    SpiInitStruct.enCPHA    = SpiMskCphafirst;//��һ���ز���
    SpiInitStruct.enCPOL    = SpiMskcpollow;  //����Ϊ��
    Spi_Init(M0P_SPI0, &SpiInitStruct);
}

void LCDSpi_Init(void)
{
	  ///< �˿ڳ�ʼ��
    App_GpioInit();
    ///< SPI��ʼ��
    App_SPIInit();
}
void LCD_IO_Init(void)
{
		stc_gpio_cfg_t stcGpioCfg;
		Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
		stcGpioCfg.enDir = GpioDirOut;
		stcGpioCfg.enPu = GpioPuDisable;
		stcGpioCfg.enPd = GpioPdDisable;
		Gpio_ClrIO(RS_Port, RS_Pin);
		Gpio_Init(RS_Port, RS_Pin, &stcGpioCfg);
		Gpio_ClrIO(LCDReset_Port, LCDReset_Pin);
		Gpio_Init(LCDReset_Port, LCDReset_Pin, &stcGpioCfg);

		LCDSpi_Init();
	
}

void LCD12864_INIT(void)
{
		LCD_IO_Init();
    Gpio_WriteOutputIO(LCDReset_Port, LCDReset_Pin, 1);
    delay1ms(100); 
    Gpio_WriteOutputIO(LCDReset_Port, LCDReset_Pin, 0);
    delay1ms(100);
		Gpio_WriteOutputIO(LCDReset_Port, LCDReset_Pin, 1);
		Spi_SetCS(M0P_SPI0, FALSE);
	
		LCD12864_SendCMD(0xAE);//--turn off oled panel
		LCD12864_SendCMD(0x00);//---set low column address
		LCD12864_SendCMD(0x10);//---set high column address
		LCD12864_SendCMD(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
		LCD12864_SendCMD(0x81);//--set contrast control register
		LCD12864_SendCMD(0xCF); // Set SEG Output Current Brightness
		LCD12864_SendCMD(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
		LCD12864_SendCMD(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
		LCD12864_SendCMD(0xA6);//--set normal display
		LCD12864_SendCMD(0xA8);//--set multiplex ratio(1 to 64)
		LCD12864_SendCMD(0x3f);//--1/64 duty
		LCD12864_SendCMD(0xD3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
		LCD12864_SendCMD(0x00);//-not offset
		LCD12864_SendCMD(0xd5);//--set display clock divide ratio/oscillator frequency
		LCD12864_SendCMD(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
		LCD12864_SendCMD(0xD9);//--set pre-charge period
		LCD12864_SendCMD(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
		LCD12864_SendCMD(0xDA);//--set com pins hardware configuration
		LCD12864_SendCMD(0x12);
		LCD12864_SendCMD(0xDB);//--set vcomh
		LCD12864_SendCMD(0x40);//Set VCOM Deselect Level
		LCD12864_SendCMD(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
		LCD12864_SendCMD(0x02);//
		LCD12864_SendCMD(0x8D);//--set Charge Pump enable/disable
		LCD12864_SendCMD(0x14);//--set(0x10) disable
		LCD12864_SendCMD(0xA4);// Disable Entire Display On (0xa4/0xa5)
		LCD12864_SendCMD(0xA6);// Disable Inverse Display On (0xa6/a7) 
		LCD12864_SendCMD(0xAF);//--turn on oled panel
		
		LCD12864_SendCMD(0xAF); /*display ON*/ 
		clear_screen();
		lcd_switch(1);
		//Lcd_Write_Time(READ_OFF(true,3600); 
}

void lcd_switch(bool state)
{
	if(state)
	{
		led_write(1);
		LCD12864_SendCMD(0xAF); /*display ON*/ 
	}
	else 
	{
		led_write(0);
		LCD12864_SendCMD(0xAE); /*display OFF*/ 
	}
}

/*******************************************************
�������ƣ�
�������ã��趨����
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void lcd_address(unsigned char page,unsigned char column)
{
    column=column-1; //����ƽ����˵�ĵ�1 �У���LCD ����IC ���ǵ�0 �С������������ȥ1.
    page=page-1;
    LCD12864_SendCMD(0xb0+page); //����ҳ��ַ��ÿҳ��8 �С�һ�������64 �б��ֳ�8 ��ҳ������ƽ����˵�ĵ�1 ҳ����LCD ����IC ���ǵ�0 ҳ�������������ȥ1*/
    LCD12864_SendCMD(((column>>4)&0x0f)+0x10); //�����е�ַ�ĸ�4 λ
    LCD12864_SendCMD(column&0x0f); //�����е�ַ�ĵ�4 λ
}

/*******************************************************
�������ƣ�ˢ������
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void full_display(void)
{
    int i,j;
    for(i=0; i<8; i++)
    {
        lcd_address(i+1,1);
        for(j=0; j<128; j++)
        {
            LCD12864_SendDATA(View_Memory[i][j]);
        }
    }
}


/*******************************************************
�������ƣ���������
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void clear_screen()
{
    unsigned char i,j;
    for(i=0; i<8; i++)
    {
        for(j=0; j<128; j++)
        {
            View_Memory[i][j]=0x00;
        }
    }
}
/*************************************************************************************************************************
* ����	:	void LCD_Fill(uint16_t xStart, uint16_t xEnd, uint16_t yStart, uint16_t yEnd)
* ����	:	ָ��λ�������ɫ
* ����	:	λ������
* ����	:	��
* ����	:	�ײ㺯��
* ����	:	cp1300@139.com
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/
void GRAM_Fill(uint16_t xStart, uint16_t yStart,uint16_t xEnd,  uint16_t yEnd)
{
    uint16_t i,j;

    for(i = xStart; i < (xEnd+1); i ++)
    {
        for(j = yStart; j < (yEnd+1); j ++)
        {
            GRAM_DrawPoint(i,j);
        }
    }
}
/*******************************************************
�������ƣ����㺯��
�������ã�ָ��λ�û��׵�
��ڲ��������� x y
���ڲ�����void
�޸ı�ע��
********************************************************/
void GRAM_ClearPoint(unsigned char x,unsigned char y)
{
    if(y >64|x>127)
        return;
    View_Memory[y / 8][x] &= ~(1 << (y % 8));
}

/*************************************************************************************************************************
* ����	:	void GRAM_Clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
* ����	:	���ָ��λ��
* ����	:	λ������
* ����	:	��
* ����	:	�ײ㺯��
* ����	:	cp1300@139.com
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/
void GRAM_Clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd)
{
    uint16_t i,j;

    for(i = xStart; i < (xEnd+1); i ++)
    {
        for(j = yStart; j < (yEnd+1); j ++)
        {
            GRAM_ClearPoint(i,j);
        }
    }
}



/*******************************************************
�������ƣ����ߺ���
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void GRAM_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
    unsigned char t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;

    //TFT_LCD_SetRamAddr(0,239,0,319);//������ʾ����

    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //���õ�������
    else if(delta_x==0)incx=0;//��ֱ��
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//ˮƽ��
    else {
        incy=-1;
        delta_y=-delta_y;
    }
    if( delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;
    for(t=0; t<=distance+1; t++ ) //�������
    {
        GRAM_DrawPoint(uRow,uCol);//����
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
/*******************************************************
�������ƣ�����ɫ�����ο�
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void ClearFrame(void)
{
    GRAM_Clear(arrowUpX,arrowUpY,arrowUpX+16,arrowUpY+8);
		GRAM_Clear(arrowDownX,arrowDownY,arrowDownX+16,arrowDownY+8);
		GRAM_Clear(MenuX,MenuY,MenuX+16*4,MenuY+16);
}
/*******************************************************
�������ƣ�����ɫ�����ο�
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void Draw_ClearRect(unsigned char Line,unsigned char Column,unsigned char Wide)
{
    for(unsigned char i=0; i<Wide; i++)
        View_Memory[Line][Column+i]=0x00;
}
/*******************************************************
�������ƣ�����ɫ�����ο�
�������ã�
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void Draw_FillRect(unsigned char Line,unsigned char Column,unsigned char Wide)
{
    for(unsigned char i=0; i<Wide; i++)
        View_Memory[Line][Column+i]=0xff;
}



/*******************************************************
�������ƣ����㺯��
�������ã�ָ��λ�û��ڵ�
��ڲ��������� x y
���ڲ�����void
�޸ı�ע��
********************************************************/
void GRAM_DrawPoint(unsigned char x,unsigned char y)
{
    if(y>64|x>127)
        return;
    View_Memory[y / 8][x] |= (1 << (y % 8));
}





/*************************************************************************************************************************
* ����	:	void GRAM_ShowChar(uint8_t x,uint8_t y,char ,FONT_MODE FontMode)
* ����	:	��ָ��λ����ʾһ��ָ����С���ַ�
* ����	:	x,y:��ʾ��ʼ����,p:���ӵ��󻺳���;
			FontMode:������ʾģʽ,
				0x80:������ʾ,0x40:����, 0:16������;1:12������
* ����	:	��
* ����	:	���㺯��
* ����	:	cp1300@139.com
* ʱ��	:	20120603
* ����޸�ʱ�� : 20120603
* ˵��	: 	FontSize = 16���� 12
*************************************************************************************************************************/
void GRAM_ShowChar(uint8_t x,uint8_t y,uint8_t CHAR,FONT_MODE FontMode)
{
    uint8_t i,j,k;
    uint8_t FontSize = (uint8_t)FontMode&0x3f;	//��ȡ�����С
    uint8_t *p;
    uint8_t rows;							//һ���ַ�ռ�ü���8��
    uint8_t column;							//������
    uint8_t temp;
    void (*DrawPoint)(uint8_t i,uint8_t j);
    void (*ClearPoint)(uint8_t i,uint8_t j);

    CHAR -= 46;
    if(CHAR > 95 - 1)
        return;


    if(FontSize==6)
    {
        rows = 1;
        column = 5;
    }
    else if(FontSize==12)
    {
        rows = 2;
        column = 8;
        //p = (uint8_t *)ASCII_8X12[CHAR];		//12��
    }
    else if(FontSize == 16)
    {
        rows = 2;
        column = 8;
				p = (uint8_t *)F8X16[CHAR];		//16��
    }
		else if(FontSize == 32)
    {
        rows = 4;
        column = 16;
				p = (uint8_t *)F16X32[CHAR];		//16��
    }
    else
    {
        rows = 2;
        column = 8;
        //p = (uint8_t *)ASCII_8X12[CHAR];		//12��
    }

    if(FontMode & 0x40)	//����
    {
        DrawPoint = GRAM_ClearPoint;
        ClearPoint =  GRAM_DrawPoint;
    }
    else //����ģʽ
    {
        ClearPoint =  GRAM_ClearPoint;
        DrawPoint =  GRAM_DrawPoint;
    }

    temp = 8;
    if(FontMode & 0x80)	//������ʾ
    {
        for(k = 0; k < rows; k ++)
        {
            for(j = 0; j < column; j ++)
            {

                for(i = 0; i < temp; i ++)
                {
                    if(*p & (1 << i))
                        (*DrawPoint)(x + j,k*8+y + i);
										else
												(*ClearPoint)(x + j,k*8+y + i);
                }

                p ++;
            }

            FontSize -= 8;
            if(FontSize >= 8) temp = 8;
            else temp = FontSize;
        }

        /*for(j = 0;j < 8;j ++)
        {
         	for(i = 0;i < FontSize - 8;i ++)
        	{
        		if(*p & (1 << i))
        			(*DrawPoint)(x + j,y + 8 + i);
        	}
        	p ++;
        }*/
    }
    else	//�ǵ�����ʾ
    {

        for(k = 0; k < rows; k ++)
        {
            for(j = 0; j < column; j ++)
            {
                for(i = 0; i < temp; i ++)
                {
                    if(*p & (1 << i))
                        (*DrawPoint)(x + j,k*8+y + i);
                    else
                        (*ClearPoint)(x + j,k*8+y + i);
                }

                p ++;
            }
            FontSize -= 8;
            if(FontSize >= 8) temp = 8;
            else temp = FontSize;
        }


        /*
        for(j = 0;j < 8;j ++)
        {
         	for(i = 0;i < FontSize - 8;i ++)
        	{
        		if(*p & (1 << i))
        			(*DrawPoint)(x + j,y + 8 + i);
        		else
        			(*ClearPoint)(x + j,y + 8 + i);
        	}
        	p ++;
        }*/
    }
}
/*************************************************************************************************************************
* ����	:	void GRAM_ShowChina(u8 x,u8 y,const u8 *p,FONT_MODE FontMode)
* ����	:	��ָ��λ����ʾһ��ָ����С�ĺ��� 
* ����	:	x,y:��ʾ��ʼ����,p:���ӵ��󻺳���;
			FontMode:������ʾģʽ,
				0x80:������ʾ,0x40:����, 0:16������;1:12������
* ����	:	��
* ����	:	���㺯��
* ����	:	cp1300@139.com
* ʱ��	:	20120531
* ����޸�ʱ�� : 20120531
* ˵��	: 	FontSize = 16���� 12
*************************************************************************************************************************/
void GRAM_ShowChina(unsigned char x,unsigned char y,const unsigned char *p,FONT_MODE FontMode)
{
	unsigned char i,j;
	unsigned char FontSize = (unsigned char)FontMode & 0x3f;	//��ȡ�����С
	void (*DrawPoint)(unsigned char i,unsigned char j);
	void (*ClearPoint)(unsigned char i,unsigned char j);
 
 
	if(FontMode & 0x40)	//����
	{
		DrawPoint = GRAM_ClearPoint;
		ClearPoint =  GRAM_DrawPoint;
	}
	else //����ģʽ
	{
		ClearPoint =  GRAM_ClearPoint;
		DrawPoint =  GRAM_DrawPoint;
	}
 
	if(FontMode & 0x80)	//������ʾ
	{
		for(i = 0;i < FontSize;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					(*DrawPoint)(x + j,y + i);
			}
			p ++;
			for(j = 0;j < FontSize - 8;j ++)
			{
				if(*p & 0x80 >> j)
					(*DrawPoint)(x + j + 8,y + i);
			}
			p ++;
		}
	}
	else	//�ǵ�����ʾ
	{
		for(i = 0;i < FontSize;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					(*DrawPoint)(x + j,y + i);
				else
					(*ClearPoint)(x + j,y + i);
			}
			p ++;
			for(j = 0;j < FontSize - 8;j ++)
			{
				if(*p & 0x80 >> j)
					(*DrawPoint)(x + j + 8,y + i);
				else
					(*ClearPoint)(x + j + 8,y + i);
			}
			p ++;
		}
	}
	
}
unsigned char * FONT_GetFontLattice(unsigned char *code)
{
    unsigned char GBKH,GBKL; 
		unsigned long ALL_GDK;
		unsigned short i;
    //uint32_t offset;        

		GBKH=*code;
		GBKL=*(code+1);
		
		//if(GBKH>0XFE||GBKH<0X81)return;
		ALL_GDK	=(unsigned short)GBKH<<8|GBKL;
		for(i=0;i<(sizeof(FontHZ_Data)/sizeof(struct Font_str));i++)
		{
			if(FontHZ_Data[i].gdk_word==ALL_GDK)
			{
				return (unsigned char *)FontHZ_Data[i].data;
			}
		}     
		return NULL;			
    
}

/*************************************************************************************************************************
* ����	:	void GRAM_ShowString(uint16_t x,uint16_t y,const uint8_t *pStr,FONT_MODE Font_MODE)
* ����	:	���Դ�ָ��λ����ʾ�ַ���
* ����	:	x,y:��ʾ��ʼ����,pStr:�ַ���������;
			FontMode:��ʾģʽ,
				0x80:������ʾ,0x40:����, 0:16������;1:12������
* ����	:	��
* ����	:	���㺯��
* ����	:	cp1300@139.com
* ʱ��	:	2014-08-20
* ����޸�ʱ�� : 2014-08-20
* ˵��	: 	FontSize = 16���� 12
*************************************************************************************************************************/
void GRAM_ShowString(uint16_t x,uint16_t y,const char *pStr,FONT_MODE Font_MODE)
{
    uint8_t Font_Size = Font_MODE & 0x3f;
    uint8_t width;
		uint8_t * font_Dat;
    switch(Font_Size)
    {
    case 6:
        width=14;
        break;
    case 12:
        width=16;
        break;
    case 16:
        width=16;
        break;
    case 24:
        width=24;
        break;
		case 32:
        width=32;
        break;
    default:
        width=16;
        break;
    }

    while(*pStr != 0)
    {
#if	CHINESE_ENABLE
        if(*pStr > 0x80)//����
        {
            font_Dat=FONT_GetFontLattice((uint8_t *)pStr);	//��ȡ���ֵ���

            GRAM_ShowChina(x,y,(const unsigned char*)font_Dat,Font_MODE);				//��ʾ����
            pStr += 2;
            if(x > 128 - 16)  //�Զ�����
            {
                x = 0;
                y += Font_Size;
            }
            else
            {
                x += width;
            }
        }
        else //ASCII
#endif
        {
            GRAM_ShowChar(x,y,*pStr,Font_MODE);
            pStr++;
            if(x > 128 - 8)  //�Զ�����
            {
                x = 0;
                y += Font_Size;
            }
            else
            {
                if(*(pStr-1)==46)x += 4;
								else x += width/2;
								if(Font_MODE==FONT32_DEFAULT)x+=1;
            }
        }

    }
}



/*************************************************************************************************************************
* ����			:	void GRAM_ShowLattice(uint16_t x,uint16_t y,uint16_t width, uint16_t height, const uint8_t LatticeBuff[], bool isInvert)
* ����			:	��ָ��λ����ʾһ��ָ����С�ĵ�������
* ����			:	x,y:��ʾ��ʼ����,width:������;height:����߶�;LatticeBuff:�������ݻ�����;isInvert:�Ƿ�ɫ��ʾ
* ����			:	��
* ����			:	���㺯��
* ����			:	cp1300@139.com
* ʱ��			:	2016-03-25
* ����޸�ʱ�� 	: 	2016-03-25
* ˵��			: 	������ʾ�Զ����ַ�
				�ַ��߶�����,��Ȳ���8�ı�����Ҫ����Ϊ8�ı������д洢,���ǲ���ʾ
				��ģȡģ��ʽ:����,����ʽ,˳��,���ֽ���ǰ
*************************************************************************************************************************/
void GRAM_ShowLattice(uint8_t x,uint8_t y,uint16_t width, uint16_t height, const uint8_t LatticeBuff[], bool isInvert)
{
    uint16_t i,j,k;
    uint8_t *p;
    void (*DrawPoint)(uint8_t i,uint8_t j);


    uint16_t w = width / 8;		//����������
    uint16_t res = width % 8;	//�����������
    p = (uint8_t *)LatticeBuff;

    if(isInvert)	//����
    {
        DrawPoint = GRAM_ClearPoint;			//����㺯��
        GRAM_Fill(x,y,x+width-1,y+height-1);	//���������
    }
    else //����ģʽ
    {
        DrawPoint =  GRAM_DrawPoint;			//���㺯��
        GRAM_Clear(x,y,x+width-1,y+height-1);	//�����ʾ����
    }


    for(i = 0; i < height; i ++)	//���л���
    {
        for(j = 0; j < w; j ++)	//��8�ı��������Ƚ��л���
        {
            for(k = 0; k < 8; k ++)
            {
                if(*p & (0x80 >> k))
                    (*DrawPoint)(x + j*8+k,y + i);
            }
            p ++;
        }
        for(k = 0; k < res; k ++)
        {
            if(*p & (0x80 >> k))
                (*DrawPoint)(x + w*8+k,y + i);
        }
        if(res > 0) p ++;

    }

    /*
    	for(i = 0;i < 16;i ++)
    	{
    		for(j = 0;j < 8;j ++)
    		{
    			if(*p & 0x80 >> j)
    				(*DrawPoint)(x + j,y + i);
    		}
    		p ++;
    		for(j = 0;j < 16 - 8;j ++)
    		{
    			if(*p & 0x80 >> j)
    				(*DrawPoint)(x + j + 8,y + i);
    		}
    		p ++;
    	}	*/
}

void clear_page_edge(uint8_t x,uint8_t y)
{
	GRAM_ClearPoint(x,y);
	GRAM_ClearPoint(x+1,y);
	GRAM_ClearPoint(x,y+1);
	
	GRAM_ClearPoint(x+23,y);
	GRAM_ClearPoint(x+23-1,y);
	GRAM_ClearPoint(x+23,y+1);
	
	GRAM_ClearPoint(x,y+23);
	GRAM_ClearPoint(x,y+23-1);
	GRAM_ClearPoint(x+1,y+23);
//	
	GRAM_ClearPoint(x+23,y+23);
	GRAM_ClearPoint(x+23-1,y+23);
	GRAM_ClearPoint(x+23,y+23-1);
}
