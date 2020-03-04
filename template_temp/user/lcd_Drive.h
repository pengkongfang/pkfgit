#ifndef _LCD_Drive_H_
#define _LCD_Drive_H_
#include "LCD12864.h"
#include "stdint.h"
#include <stdbool.h>

#define WM_CREATE                  	0x0000
#define WM_UP                   		0x0001
#define WM_DOWN                  		0x0002
#define WM_M                   			0x0003
#define WM_1                   			0x0004
#define WM_2                   			0x0005
#define WM_3                 			  0x0006
#define WM_4                 			  0x0007
#define WM_TIMEOUT                  0x0008
#define WM_LONG                  		0x0009
#define WM_OFF                  		0x000A
#define WM_START                  	0x000B


#define READ_BK  1
#define READ_OFF 2

//#define GUI_COUNTOF(a)          (sizeof(a) / sizeof(a[0]))
#define Line_Top   2
#define Line_Down  55
#define Line_1  12
#define Line_2  22
#define Line_3  32
#define Line_4  42
typedef struct WM_MESSAGE WM_MESSAGE;
typedef void (*WM_CALLBACK)(int pMsg);
struct WM_MESSAGE {
    int MsgId;            /* type of message */
    union {
        const void * p;            /* Some messages need more info ... Pointer is declared "const" because some systems (M16C) have 4 byte const, byte 2 byte default ptrs */
        int v;
    } Data;
};

typedef struct {
    unsigned char Now_Focus;
    unsigned char Max_Focus_Num;
    char * Focus_Data[5];
} Focus_Srtuct_Typdef;

typedef struct {
   uint16_t OFF_Time;
	 uint16_t Back_Time;
	 uint16_t MaxBac_Time;
	 bool Back_TimeOut_Flag;
	 bool OFF_TimeOut_Flag;
	 uint16_t OFF_Time_Set;
}LCDTimeOut_Srtuct_Typdef;

struct WM_Obj {
    WM_CALLBACK  cb;      /* Ptr to notification callback */
};

typedef struct 
{
	void (*init)(void);
	void (*write)(void);
	int  (*read)(int msg); 
	void (*open)(WM_CALLBACK  Frame);
	void (*control)(int pMsg);
	void  (*intrrpt_cb)(void); 
	LCDTimeOut_Srtuct_Typdef LCDTimeOut_Str;
}lcd_str;



extern lcd_str Lcd;
extern struct WM_Obj WM_Frame;
void Set_Focus(Focus_Srtuct_Typdef *Focus,int Num);
void Creat_Frame(WM_CALLBACK  Frame);
void GRAM_Clear(uint16_t xStart, uint16_t yStart, uint16_t xEnd, uint16_t yEnd);
void GetLang(Focus_Srtuct_Typdef *Focus,const char * Source_aLang[][2]);
void Gui_Init(void);
void SW_OFF_Time(bool state,uint16_t OFF_Time);
void SW_Back_Time(bool state,uint8_t Second);
void DisChar_InFill(unsigned int x, unsigned int y ,char *Buff);
void DisNum_InFill(unsigned int x, unsigned int y ,unsigned char num,bool Mode);
void LCD_Interrupt(void);
void LCD_Display(void);
#endif

