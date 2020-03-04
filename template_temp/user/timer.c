#include "timer.h"
#include "key.h"
#include "bm43.h"


/*******************************************************************************
 * BT1�жϷ�����
 ******************************************************************************/
void Bt2Int(void)
{
    if (TRUE == Bt_GetIntFlag(TIM2))
    {
				KEY_Time_Interrupt();
				bm43_interrpt();
        Bt_ClearIntFlag(TIM2);
    }
}

en_result_t timer2_init(void)
{
		stc_bt_config_t   stcConfig;
    en_result_t       enResult = Ok;
    uint16_t          u16ArrData = 0x8000;
    uint16_t          u16InitCntData = 0xC000;
     Clk_SetPeripheralGate(ClkPeripheralBt, TRUE);
    stcConfig.pfnTim2Cb = Bt2Int;
    //P25����Ϊ�ſ�ʹ��IO
        
    stcConfig.enPRS   = BtPCLKDiv16;
    stcConfig.enTog   = BtTogDisable;
    stcConfig.enCT    = BtTimer;
    stcConfig.enMD    = BtMode2;
    //Bt��ʼ��
    if (Ok != Bt_Init(TIM2, &stcConfig))
    {
        enResult = Error;
    }
    
    //TIM2�ж�ʹ��
    Bt_ClearIntFlag(TIM2);
    Bt_EnableIrq(TIM2);
    EnableNvic(TIM2_IRQn, 3, TRUE);
    
    //��������ֵ�ͼ���ֵ����������
    Bt_ARRSet(TIM2, u16ArrData);
    Bt_Cnt16Set(TIM2, u16InitCntData);
    Bt_Run(TIM2);
		return enResult;
}


