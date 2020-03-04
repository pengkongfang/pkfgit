#include "timer.h"



/*******************************************************************************
 * BT1中断服务函数
 ******************************************************************************/
void Tim2_IRQHandler(void)
{
    if(TRUE == Bt_GetIntFlag(TIM2, BtUevIrq))
    {
				LCD_Interrupt();
				KEY_Time_Interrupt();
				Beep_Interrupt();
				Bt_ClearIntFlag(TIM2,BtUevIrq); //中断标志清零
    }
}

void App_Timer2Cfg(uint16_t u16Period)
{
		uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
    
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE); //Base Timer外设时钟使能
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //定时器模式
    stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv32;                 //PCLK/256
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //自动重载16位计数器/定时器
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM2, &stcBtBaseCfg);                     //TIM0 的模式0功能初始化
    
    u16ArrValue = 0xC000 - u16Period;
    Bt_M0_ARRSet(TIM2, u16ArrValue);                        //设置重载值(ARR = 0x10000 - 周期)
    
    u16CntValue = 0xC000 - u16Period;
    Bt_M0_Cnt16Set(TIM2, u16CntValue);                      //设置计数初值
    
    Bt_ClearIntFlag(TIM2,BtUevIrq);                         //清中断标志   
    Bt_Mode0_EnableIrq(TIM2);                               //使能TIM0中断(模式0时只有一个中断)
    EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);                 //TIM0中断使能
}


void timer2_init(void)
{
    App_Timer2Cfg(0x9B61);   //Timer0配置初始化(周期 = 48000*(1/48M)*256 = 256ms)
    Bt_M0_Run(TIM2);        //TIM0 运行。
}


