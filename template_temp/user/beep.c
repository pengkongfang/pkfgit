#include "beep.h"
#include "adt.h"
#include "lpm.h"

beep_str Beep;

en_adt_unit_t        enAdt;
en_result_t  beep_init(void)
{
	 
    uint16_t                u16Period;
    en_adt_compare_t        enAdtCompareA;
    uint16_t                u16CompareA;
    stc_adt_basecnt_cfg_t   stcAdtBaseCntCfg;
    stc_adt_CHxX_port_cfg_t stcAdtTIM6ACfg;
    
    DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
    DDL_ZERO_STRUCT(stcAdtTIM6ACfg);

    
    Clk_SwitchTo(ClkRCH);
    
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE); //端口外设时钟使能
        
    Gpio_SetFunc_TIM6_CHA_P02();

    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdt, TRUE))//ADT外设时钟使能
    {
        return Error;
    }
    
    enAdt = AdTIM4;
    
    stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
    stcAdtBaseCntCfg.enCntDir = AdtCntUp;
    stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div4;
    
    Adt_Init(enAdt, &stcAdtBaseCntCfg);                      //ADT载波、计数模式、时钟配置
    
    u16Period = 0xC000;
    
    Adt_SetPeriod(enAdt, u16Period);                         //周期设置
    
    enAdtCompareA = AdtCompareA;
    u16CompareA = 0x6000;
    
    Adt_SetCompareValue(enAdt, enAdtCompareA, u16CompareA);  //通用比较基准值寄存器A设置
    
    stcAdtTIM6ACfg.enCap = AdtCHxCompareOutput;
    stcAdtTIM6ACfg.bOutEn = TRUE;
    stcAdtTIM6ACfg.enPerc = AdtCHxPeriodLow;
    stcAdtTIM6ACfg.enCmpc = AdtCHxCompareHigh;
    stcAdtTIM6ACfg.enStaStp = AdtCHxStateSelSS;
    stcAdtTIM6ACfg.enStaOut = AdtCHxPortOutLow;
    Adt_CHxXPortConfig(enAdt, AdtCHxA, &stcAdtTIM6ACfg);    //端口CHA配置
    
    
    Adt_StartCount(enAdt);
		return Ok;
}

void beep_write(bool sta)
{
	if(sta)Adt_StartCount(enAdt);
	else Adt_StopCount(enAdt);
}


