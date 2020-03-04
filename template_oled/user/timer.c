#include "timer.h"



/*******************************************************************************
 * BT1�жϷ�����
 ******************************************************************************/
void Tim2_IRQHandler(void)
{
    if(TRUE == Bt_GetIntFlag(TIM2, BtUevIrq))
    {
				LCD_Interrupt();
				KEY_Time_Interrupt();
				Beep_Interrupt();
				Bt_ClearIntFlag(TIM2,BtUevIrq); //�жϱ�־����
    }
}

void App_Timer2Cfg(uint16_t u16Period)
{
		uint16_t                  u16ArrValue;
    uint16_t                  u16CntValue;
    stc_bt_mode0_cfg_t     stcBtBaseCfg;
    
    DDL_ZERO_STRUCT(stcBtBaseCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBaseTim, TRUE); //Base Timer����ʱ��ʹ��
    
    stcBtBaseCfg.enWorkMode = BtWorkMode0;                  //��ʱ��ģʽ
    stcBtBaseCfg.enCT       = BtTimer;                      //��ʱ�����ܣ�����ʱ��Ϊ�ڲ�PCLK
    stcBtBaseCfg.enPRS      = BtPCLKDiv32;                 //PCLK/256
    stcBtBaseCfg.enCntMode  = Bt16bitArrMode;               //�Զ�����16λ������/��ʱ��
    stcBtBaseCfg.bEnTog     = FALSE;
    stcBtBaseCfg.bEnGate    = FALSE;
    stcBtBaseCfg.enGateP    = BtGatePositive;
    Bt_Mode0_Init(TIM2, &stcBtBaseCfg);                     //TIM0 ��ģʽ0���ܳ�ʼ��
    
    u16ArrValue = 0xC000 - u16Period;
    Bt_M0_ARRSet(TIM2, u16ArrValue);                        //��������ֵ(ARR = 0x10000 - ����)
    
    u16CntValue = 0xC000 - u16Period;
    Bt_M0_Cnt16Set(TIM2, u16CntValue);                      //���ü�����ֵ
    
    Bt_ClearIntFlag(TIM2,BtUevIrq);                         //���жϱ�־   
    Bt_Mode0_EnableIrq(TIM2);                               //ʹ��TIM0�ж�(ģʽ0ʱֻ��һ���ж�)
    EnableNvic(TIM2_IRQn, IrqLevel3, TRUE);                 //TIM0�ж�ʹ��
}


void timer2_init(void)
{
    App_Timer2Cfg(0x9B61);   //Timer0���ó�ʼ��(���� = 48000*(1/48M)*256 = 256ms)
    Bt_M0_Run(TIM2);        //TIM0 ���С�
}


