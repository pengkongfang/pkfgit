#include "Lcd_Drive.h"

struct WM_Obj WM_Frame;
lcd_str Lcd;
bool MyLanguage;


/*******************************************************
�������ƣ����������ʼ��
�������ã���ʼ������Ľ���
��ڲ�����void
���ڲ�����void
�޸ı�ע��
********************************************************/
void Gui_Init(void)
{

}

/*******************************************************
�������ƣ���ȡ����
�������ã���ȡ��������
��ڲ���������ָ��  ��������
���ڲ�����void
�޸ı�ע��
********************************************************/
void GetLang(Focus_Srtuct_Typdef *Focus,const char * Source_aLang[][2])
{
				unsigned char i=0;
        for(i=0; i<Focus->Max_Focus_Num; i++)
        Focus->Focus_Data[i]=(char*)Source_aLang[i][MyLanguage];
	
				Focus->Focus_Data[i]=(char*)Source_aLang[Focus->Max_Focus_Num][MyLanguage];
				
}

/*******************************************************
�������ƣ�
�������ã��ڻ�����������ʾ����
��ڲ�����X Y ��ʾ������  ģʽ�����Ի������ԣ�
���ڲ�����void
�޸ı�ע��
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
�������ƣ�
�������ã��ڻ�����������ʾ�ַ���
��ڲ�����X Y ��ʾ�����ݣ����ԣ�
���ڲ�����void
�޸ı�ע��
********************************************************/
void DisChar_InFill(unsigned int x, unsigned int y ,char *Buff)
{

	GRAM_Fill(x,y,x+20,y+10);
	GRAM_ShowString(x+1,y+2,Buff,FONT6_REVERSE);

}





