#include "beep.h"
#include "ddl.h"
#include "bt.h"
#include "gpio.h"
#include "flash.h"
#include "adc.h"
#include "user_sever.h"
Beep_Struct_Typdef Beep_Struct;


//Timer0 ����
/*******************************************************************************
 * TIM0�жϷ�����
 ******************************************************************************/

//�˿����ó�ʼ��
void App_Timer0PortCfg(void)
{
    stc_gpio_cfg_t             stcTIM0Port;
    stc_gpio_cfg_t             stcLEDPort;
    
    //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcTIM0Port);
    DDL_ZERO_STRUCT(stcLEDPort);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO ����ʱ��ʹ��
    
    stcLEDPort.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcLEDPort);     //PD14����ΪLED���
    
    stcTIM0Port.enDir  = GpioDirOut;
    
    
    Gpio_Init(GpioPortA, GpioPin1, &stcTIM0Port);
    Gpio_SetAfMode(GpioPortA,GpioPin1,GpioAf3);            //PA01����ΪTIM0_CHB
}


void App_Timer0Cfg(uint16_t u16Period, uint16_t u16CHxACompare, uint16_t u16CHxBCompare)
{
    uint16_t                   u16CntValue;
   // uint8_t                    u8ValidPeriod;
    stc_bt_mode23_cfg_t        stcBtBaseCfg;
    stc_bt_m23_compare_cfg_t   stcBtPortCmpCfg;
    
    //�ṹ���ʼ������
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    DDL_ZERO_STRUCT(stcBtPortCmpCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE);   //Base Timer����ʱ��ʹ��
        
    stcBtBaseCfg.enWorkMode    = BtWorkMode2;              //��ݲ�ģʽ
    stcBtBaseCfg.enCT          = BtTimer;                  //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS         = BtPCLKDiv1;               //PCLK
    stcBtBaseCfg.enCntDir      = BtCntUp;                  //���ϼ����������ǲ�ģʽʱֻ��
    stcBtBaseCfg.enPWMTypeSel  = BtIndependentPWM;         //�������PWM
    stcBtBaseCfg.enPWM2sSel    = BtSinglePointCmp;         //����ȽϹ���
    stcBtBaseCfg.bOneShot      = FALSE;                    //ѭ������
    stcBtBaseCfg.bURSSel       = FALSE;                    //���������
    Bt_Mode23_Init(TIM0, &stcBtBaseCfg);                   //TIM0 ��ģʽ23���ܳ�ʼ��
    
    Bt_M23_ARRSet(TIM0, u16Period, TRUE);                  //��������ֵ,��ʹ�ܻ���
  
    Bt_M23_CCR_Set(TIM0, BtCCR0B, u16CHxBCompare);         //���ñȽ�ֵB
       
    stcBtPortCmpCfg.enCH0BCmpCtrl   = BtPWMMode2;          //OCREFB�������OCMB:PWMģʽ2
    stcBtPortCmpCfg.enCH0BPolarity  = BtPortPositive;      //�������
    stcBtPortCmpCfg.bCH0BCmpBufEn   = TRUE;                //Bͨ���������ʹ��
    stcBtPortCmpCfg.enCH0BCmpIntSel = BtCmpIntNone;        //Bͨ���ȽϿ���:��
    
    Bt_M23_PortOutput_Cfg(TIM0, &stcBtPortCmpCfg);         //�Ƚ�����˿�����
        
    //u8ValidPeriod = 0;                                     //�¼������������ã�0��ʾ��ݲ�ÿ�����ڸ���һ�Σ�ÿ+1�����ӳ�1������
    //Bt_M23_SetValidPeriod(TIM0,u8ValidPeriod);             //�����������
        
   // u16CntValue = 0;
    Bt_M23_Cnt16Set(TIM0, u16CntValue);                    //���ü�����ֵ
    
    Bt_ClearAllIntFlag(TIM0);                              //���жϱ�־
   // EnableNvic(TIM0_IRQn, IrqLevel0, TRUE);                //TIM0�ж�ʹ��
    //Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                    //ʹ��TIM0 UEV�����ж�
}

void beep_init(void)
{
	 App_Timer0Cfg(24000, 12000, 12000);   //Timer0 ����:���� 48000(��ݲ�: 1K); ͨ��A�Ƚ�ֵ24000; ͨ��B�Ƚ�ֵ24000
    
	 App_Timer0PortCfg();                  //Timer3 Port�˿�����
	
	 Bt_M23_EnPWM_Output(TIM0, FALSE, FALSE);    //TIM0 �˿����ʹ��
	
	 Bt_M23_Run(TIM0);                          //TIM0 ����
}

void beep_write(bool sta)
{
	if(sta)Bt_M23_EnPWM_Output(TIM0, TRUE, FALSE);    //TIM0 �˿����ʹ��
	else Bt_M23_EnPWM_Output(TIM0, FALSE, FALSE);    //TIM0 �˿����ʹ�� 
}

void led_init(void)
{
	stc_gpio_cfg_t stcGpioCfg;
	///< ��GPIO����ʱ���ſ�
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
	///< �˿ڷ�������->���(�������������ϣ����룩���ò���һ��)
	stcGpioCfg.enDir = GpioDirOut;
	///< �˿�����������->����
	stcGpioCfg.enPu = GpioPuDisable;
	stcGpioCfg.enPd = GpioPdDisable;
	///< LED�ر�
	Gpio_SetIO(GpioPortD, GpioPin1);
	///< GPIO IO LED�˿ڳ�ʼ��
	Gpio_Init(GpioPortD, GpioPin1, &stcGpioCfg);
	
	Gpio_SetIO(GpioPortA, GpioPin0);
	///< GPIO IO LED�˿ڳ�ʼ��
	Gpio_Init(GpioPortA, GpioPin0, &stcGpioCfg);
}

void led_write(bool sta)
{
	if(sta)
	{
		Gpio_SetIO(GpioPortD, GpioPin1);
	}
	else
	{
		Gpio_ClrIO(GpioPortD, GpioPin1);
	}
	//Gpio_SetIO(0, 3, (sta)?TRUE:FALSE);
}





//void Beep_Interrupt(void)
//{
//	if(Beep_Struct.Beep_Num)
//	{
//		if(Beep_Struct.Beep_State&&Beep_Struct.Beep_ONTime)
//		{
//			Beep_Struct.Beep_ONTime--;
//			if(!Beep_Struct.Beep_ONTime)
//			{
//				Beep_Close
//				Beep_Struct.Beep_State=false;
//				Temp_Beep_OFFTime=Beep_Struct.Beep_OFFTime;
//			}
//		}
//		else if(Beep_Struct.Beep_OFFTime&&!Beep_Struct.Beep_State)
//		{
//			Beep_Struct.Beep_OFFTime--;
//			if(!Beep_Struct.Beep_OFFTime)
//			{
//					Beep_Struct.Beep_Num--;
//					if(!Beep_Struct.Beep_Num)return ;
//					Beep_Open
//					Beep_Struct.Beep_State=true;
//					Temp_Beep_OFFTime=Beep_Struct.Beep_OFFTime;
//			}
//		}
//		else
//		{
//			Beep_Struct.Beep_Num=0;
//		}
//	}

//}

void Beep_Deal(void)
{
	if(Beep_Struct.Beep_Num)
	{
		if(Beep_Struct.Beep_State&&Beep_Struct.Beep_ONTime)
		{
			Beep_Struct.Beep_Delay_State=true;
			Beep_Open
			if(Beep_Struct.Beep_Delay_Time>=Beep_Struct.Beep_ONTime)
			{
				Beep_Struct.Beep_Delay_Time=0;
				Beep_Close
				Beep_Struct.Beep_State=false;
			}
		}
		else if(!Beep_Struct.Beep_State&&Beep_Struct.Beep_OFFTime)
		{
			if(Beep_Struct.Beep_Delay_Time>=Beep_Struct.Beep_OFFTime)
			{
				Beep_Struct.Beep_Num--;
				if(Beep_Struct.Beep_Num)
				{
					Beep_Struct.Beep_Delay_Time=0;
					Beep_Struct.Beep_State=true;
					Beep_Open
				}
				else
				{
					Beep_Struct.Beep_Delay_State=0;
					Beep_Struct.Beep_Delay_Time=0;
					Beep_Close
				}
			}
		}
		else
		{
			Beep_Close
			Beep_Struct.Beep_Num=0;
			Beep_Struct.Beep_Delay_State=0;
			Beep_Struct.Beep_Delay_Time=0;
		}
	}
	else
	{
			Beep_Struct.Beep_Delay_State=0;
			Beep_Struct.Beep_Delay_Time=0;
	}
}
void Beep_Interrupt(void)
{
	if(Beep_Struct.Beep_Delay_State)Beep_Struct.Beep_Delay_Time++;
}



void Beep_SetNum(uint8_t time,uint8_t ONTime,uint8_t OFFTime)
{
	if(!Para_read(MODE_REMINDER)&0x01)return ;
	Beep_Struct.Beep_State=true;
	Beep_Struct.Beep_Delay_Time=0;
	Beep_Struct.Beep_Num=time;
	Beep_Struct.Beep_ONTime=ONTime;
	Beep_Struct.Beep_OFFTime=OFFTime;
}

