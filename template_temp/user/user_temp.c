#include "user_temp.h"
#include "i2c.h"
#include "gpio.h"
#include "adc.h"
#include "math.h"
#include "bm43.h"
#include "my_iic.h"
#include "user_sever.h"
uint8_t u8Senddata[10] = {0x12,0x34,0x77,0x66,0x55,0x44,0x33,0x22,0x11,0x00};
uint8_t u8Recdata[10]={0x00};
uint8_t u8DevAddr = 0x50;
void user_i2c_init(void)
{
		stc_i2c_config_t stcI2cCfg;
    stc_clk_config_t stcCfg;
	
    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcI2cCfg);
    //时钟部分配置
    stcCfg.enClkSrc = ClkRCH;
    stcCfg.enHClkDiv = ClkDiv1;
    stcCfg.enPClkDiv = ClkDiv1;
    Clk_Init(&stcCfg);

    Gpio_InitIOExt(1,4,GpioDirOut,FALSE,FALSE,TRUE,FALSE);   
    Gpio_InitIOExt(1,5,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
    
    Gpio_SetFunc_I2CDAT_P15(); 
    Gpio_SetFunc_I2C_SCL_P14();


    Clk_SetPeripheralGate(ClkPeripheralI2c,TRUE);
    
    stcI2cCfg.enFunc = I2cBaud_En;
    stcI2cCfg.u8Tm = 0x01;//1M波特率配置
    stcI2cCfg.pfnI2cCb = NULL;
    stcI2cCfg.bTouchNvic = FALSE;
    
    I2C_DeInit();
    
    I2C_Init(&stcI2cCfg);
    I2C_SetFunc(I2cHlm_En);
    I2C_SetFunc(I2cMode_En);


}

void I2C_Write_Data(uint8_t u8DevAddr,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
	I2C_MasterWriteData((u8DevAddr),u8Addr,NULL,0);
}

void I2C_Read_Data(uint8_t u8DevAddr,uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{
	I2C_MasterReadData(u8DevAddr,pu8Data,u8Addr,u32Len);
}

float Get_BM43Temp(void)
{
	float Temp;
	Temp=Get_BM43_Temp();
	return Temp;
}




int32_t user_adc_init(void)
{
		stc_adc_cfg_t      stcAdcCfg;
    stc_adc_scan_cfg_t stcAdcScanCfg;

    
    DDL_ZERO_STRUCT(stcAdcCfg);
    DDL_ZERO_STRUCT(stcAdcScanCfg);
    
      
    Clk_SetPeripheralGate(ClkPeripheralGpio, TRUE);              //GPIO 外设时钟使能
    
    if (Ok != Clk_SetPeripheralGate(ClkPeripheralAdcBgr, TRUE))  //ADCBGR 外设时钟使能
    {
        return Error;
    }

    Gpio_SetAnalog(3, 4, TRUE);

    
    Adc_Enable();
    M0P_BGR->CR_f.BGR_EN = 0x1u;   //BGR必须使能
    M0P_BGR->CR_f.TS_EN = 0x1u;    //内置温度传感器，视使用需求
    delay100us(1);
    
    stcAdcCfg.enAdcOpMode = AdcScanMode;             //扫描采样模式
    stcAdcCfg.enAdcClkSel = AdcClkSysTDiv2;            //PCLK
    stcAdcCfg.enAdcSampTimeSel = AdcSampTime4Clk;
    //stcAdcCfg.enAdcRefVolSel = RefVolSelInBgr2p5;    //参考电压:内部2.5V(avdd>3V,SPS<=200kHz)
    stcAdcCfg.enAdcRefVolSel = RefVolSelAVDD;          //参考电压:AVDD
    stcAdcCfg.bAdcInBufEn = FALSE;                     //电压跟随器如果使能，SPS采样速率 <=200K
    stcAdcCfg.enAdcTrig0Sel = AdcTrigDisable;          //ADC转换自动触发设置
    stcAdcCfg.enAdcTrig1Sel = AdcTrigDisable;
    Adc_Init(&stcAdcCfg);
    
    stcAdcScanCfg.u8AdcScanModeCh = ADC_SCAN_CH5_EN ;
    stcAdcScanCfg.u8AdcSampCnt = 1;   //0                //连续扫描转换次数，保持通道的倍数，6通道 = 0x5+1(1倍)，或者11+1(2倍)……
    Adc_ConfigScanMode(&stcAdcCfg, &stcAdcScanCfg);    
		return Ok;
}

//uint16_t AD_Value_Adjust(uint16_t VB_Value,uint16_t Cur_Value)
//{
//	float 	Valtage_tolerrance;
//	uint16_t Real_AD_Value_After_Adjust;
//	uint16_t temp;
//}

void NTC_temp_Work(void)
{
	uint16_t u16ScanResult=0;
	static uint32_t u16ScanAll;
	static uint8_t  count;
	Adc_Start();  
	while(FALSE == M0P_ADC->IFR_f.CONT_INTF);
	M0P_ADC->ICLR_f.CONT_INTC = 0;
	Adc_GetScanResult(2, &u16ScanResult);
	u16ScanAll+=u16ScanResult;
	count++;
	if(count>=20)
	{
		count=0;
		u16ScanAll=0;
		float NTC_ADC_Voltage=((float)u16ScanAll/50.0f)*3.3f/4096.0f;
		float Resistance=((10000.0f*(3.3f-NTC_ADC_Voltage))/NTC_ADC_Voltage)/10000.0f;
		User_Dat.NTF_Temp=(float)((1.0f/(log(Resistance)/3590.0f+1.0f/298.15f))-273.15f);
	}
}

float Get_NTC_Temp(void)
{
	return User_Dat.NTF_Temp;
}


