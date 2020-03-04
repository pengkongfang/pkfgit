#include "beep.h"
#include "ddl.h"
#include "bt.h"
#include "gpio.h"
#include "flash.h"
#include "adc.h"
#include "user_sever.h"
Beep_Struct_Typdef Beep_Struct;


//Timer0 配置
/*******************************************************************************
 * TIM0中断服务函数
 ******************************************************************************/

//端口配置初始化
void App_Timer0PortCfg(void)
{
    stc_gpio_cfg_t             stcTIM0Port;
    stc_gpio_cfg_t             stcLEDPort;
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcTIM0Port);
    DDL_ZERO_STRUCT(stcLEDPort);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); //GPIO 外设时钟使能
    
    stcLEDPort.enDir  = GpioDirOut;
    Gpio_Init(STK_LED_PORT, STK_LED_PIN, &stcLEDPort);     //PD14设置为LED输出
    
    stcTIM0Port.enDir  = GpioDirOut;
    
    
    Gpio_Init(GpioPortA, GpioPin1, &stcTIM0Port);
    Gpio_SetAfMode(GpioPortA,GpioPin1,GpioAf3);            //PA01设置为TIM0_CHB
}


void App_Timer0Cfg(uint16_t u16Period, uint16_t u16CHxACompare, uint16_t u16CHxBCompare)
{
    uint16_t                   u16CntValue;
   // uint8_t                    u8ValidPeriod;
    stc_bt_mode23_cfg_t        stcBtBaseCfg;
    stc_bt_m23_compare_cfg_t   stcBtPortCmpCfg;
    
    //结构体初始化清零
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    DDL_ZERO_STRUCT(stcBtPortCmpCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE);   //Base Timer外设时钟使能
        
    stcBtBaseCfg.enWorkMode    = BtWorkMode2;              //锯齿波模式
    stcBtBaseCfg.enCT          = BtTimer;                  //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS         = BtPCLKDiv1;               //PCLK
    stcBtBaseCfg.enCntDir      = BtCntUp;                  //向上计数，在三角波模式时只读
    stcBtBaseCfg.enPWMTypeSel  = BtIndependentPWM;         //独立输出PWM
    stcBtBaseCfg.enPWM2sSel    = BtSinglePointCmp;         //单点比较功能
    stcBtBaseCfg.bOneShot      = FALSE;                    //循环计数
    stcBtBaseCfg.bURSSel       = FALSE;                    //上下溢更新
    Bt_Mode23_Init(TIM0, &stcBtBaseCfg);                   //TIM0 的模式23功能初始化
    
    Bt_M23_ARRSet(TIM0, u16Period, TRUE);                  //设置重载值,并使能缓存
  
    Bt_M23_CCR_Set(TIM0, BtCCR0B, u16CHxBCompare);         //设置比较值B
       
    stcBtPortCmpCfg.enCH0BCmpCtrl   = BtPWMMode2;          //OCREFB输出控制OCMB:PWM模式2
    stcBtPortCmpCfg.enCH0BPolarity  = BtPortPositive;      //正常输出
    stcBtPortCmpCfg.bCH0BCmpBufEn   = TRUE;                //B通道缓存控制使能
    stcBtPortCmpCfg.enCH0BCmpIntSel = BtCmpIntNone;        //B通道比较控制:无
    
    Bt_M23_PortOutput_Cfg(TIM0, &stcBtPortCmpCfg);         //比较输出端口配置
        
    //u8ValidPeriod = 0;                                     //事件更新周期设置，0表示锯齿波每个周期更新一次，每+1代表延迟1个周期
    //Bt_M23_SetValidPeriod(TIM0,u8ValidPeriod);             //间隔周期设置
        
   // u16CntValue = 0;
    Bt_M23_Cnt16Set(TIM0, u16CntValue);                    //设置计数初值
    
    Bt_ClearAllIntFlag(TIM0);                              //清中断标志
   // EnableNvic(TIM0_IRQn, IrqLevel0, TRUE);                //TIM0中断使能
    //Bt_Mode23_EnableIrq(TIM0,BtUevIrq);                    //使能TIM0 UEV更新中断
}

void beep_init(void)
{
	 App_Timer0Cfg(24000, 12000, 12000);   //Timer0 配置:周期 48000(锯齿波: 1K); 通道A比较值24000; 通道B比较值24000
    
	 App_Timer0PortCfg();                  //Timer3 Port端口配置
	
	 Bt_M23_EnPWM_Output(TIM0, FALSE, FALSE);    //TIM0 端口输出使能
	
	 Bt_M23_Run(TIM0);                          //TIM0 运行
}

void beep_write(bool sta)
{
	if(sta)Bt_M23_EnPWM_Output(TIM0, TRUE, FALSE);    //TIM0 端口输出使能
	else Bt_M23_EnPWM_Output(TIM0, FALSE, FALSE);    //TIM0 端口输出使能 
}

void led_init(void)
{
	stc_gpio_cfg_t stcGpioCfg;
	///< 打开GPIO外设时钟门控
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); 
	///< 端口方向配置->输出(其它参数与以上（输入）配置参数一致)
	stcGpioCfg.enDir = GpioDirOut;
	///< 端口上下拉配置->下拉
	stcGpioCfg.enPu = GpioPuDisable;
	stcGpioCfg.enPd = GpioPdDisable;
	///< LED关闭
	Gpio_SetIO(GpioPortD, GpioPin1);
	///< GPIO IO LED端口初始化
	Gpio_Init(GpioPortD, GpioPin1, &stcGpioCfg);
	
	Gpio_SetIO(GpioPortA, GpioPin0);
	///< GPIO IO LED端口初始化
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

