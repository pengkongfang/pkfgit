#ifndef _LCD12864_H_
#define _LCD12864_H_

#include "LCD_Drive.h"
#include "ddl.h"
#include "spi.h"
//#include "tim.h"
#include "gpio.h"
#include "stdbool.h"
#define RS_Port  (3)
#define RS_Pin   (3)

#define LCDReset_Port  (3)
#define LCDReset_Pin   (2)



#define LCD12864_CMD_MODE   Gpio_SetIO(RS_Port, RS_Pin, 0);
#define LCD12864_DATA_MODE  Gpio_SetIO(RS_Port, RS_Pin, 1);

#define English_Mode 0
#define German_Mode  1

#define Black_Word  0
#define White_Word  1

#define LCD12864_SendCMD(cmd) {LCD12864_CMD_MODE; Spi_SendData(cmd);}
#define LCD12864_SendDATA(data) {LCD12864_DATA_MODE; Spi_SendData(data);}



void Draw_ClearRect(unsigned char Line,unsigned char Column,unsigned char Wide);
void Draw_FillRect(unsigned char Line,unsigned char Column,unsigned char Wide);
void GRAM_DrawPoint(unsigned char x,unsigned char y);
void display_graphic_32x32(unsigned char page,unsigned char column,unsigned char *dp);
void display_string_5x7(unsigned char page,unsigned char column,char *text,bool Mode);
void GRAM_DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void Drew_Top_Arrow(unsigned char Line ,unsigned char Column);
void full_display(void);
void clear_screen(void);

void LCD12864_INIT(void);
extern unsigned char View_Memory[8][128];

void ClearFrame(void);
typedef enum
{
    FONT24_DEFAULT 	= (0x80+24),	//24∫≈,µ˛º”œ‘ æ
    FONT16_DEFAULT 	= (0x80+16),	//16∫≈,µ˛º”œ‘ æ
    FONT12_DEFAULT 	= (0x80+12),	//12∫≈,µ˛º”œ‘ æ
    FONT6_DEFAULT 	= (0x80+6),	//12∫≈,µ˛º”œ‘ æ
    FONT24_COVER 	= 	(24),		//24∫≈,∏≤∏«œ‘ æ
    FONT16_COVER 	= 	(16),		//16∫≈,∏≤∏«œ‘ æ
    FONT12_COVER 	= 	(12),		//12∫≈,∏≤∏«œ‘ æ
    FONT6_COVER 	= 	(6),		//6∫≈,∏≤∏«œ‘ æ
    FONT24_REVERSE 	= (0x40+24),	//24∫≈,∑¥œ‘œ‘ æ
    FONT16_REVERSE 	= (0x40+16),	//16∫≈,∑¥œ‘œ‘ æ
    FONT12_REVERSE 	= (0x40+12),	//12∫≈,∑¥œ‘œ‘ æ
    FONT6_REVERSE 	= (0x40+6),	//12∫≈,µ˛º”œ‘ æ
} FONT_MODE;

void LCD12864_Power(bool Set);
void GRAM_ShowLattice(uint8_t x,uint8_t y,uint16_t width, uint16_t height, const uint8_t LatticeBuff[], bool isInvert);
void GRAM_Fill(uint16_t xStart, uint16_t yStart,uint16_t xEnd,  uint16_t yEnd);
void GRAM_ShowChar(uint8_t x,uint8_t y,uint8_t CHAR,FONT_MODE Font_MODE);
void GRAM_ShowString(uint16_t x,uint16_t y,const char *pStr,FONT_MODE Font_MODE);
#endif

