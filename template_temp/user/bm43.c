#include "bm43.h"
#include "ddl.h"
#include "math.h"
#include "stdbool.h"

struct bm43_Str bm43_str;


unsigned char SlaveAddr = 0x00; //Change after changing slaveaddress 0x02 register[6:0]


__weak void I2C_Write_Data(uint8_t u8DevAddr,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
	
}

__weak void I2C_Read_Data(uint8_t u8DevAddr,uint8_t *pu8Data,uint8_t u8Addr,uint32_t u32Len)
{

}

static void Get_Bm43Data(uint8_t *Register)
{
	uint8_t GetDat[7]={0};
	switch(bm43_str.deal_index)
	{
		case 0:
			I2C_Write_Data((SlaveAddr<<1),Start_CM,NULL,0);
			bm43_str.deal_index++;
			bm43_str.delay_state=true;
			break;
		case 1:
			if(bm43_str.delay_time>1)
			{
				bm43_str.delay_time=0;
				bm43_str.delay_state=false;
				bm43_str.deal_index++;
			}
			break;
		case 2:
			I2C_Write_Data((SlaveAddr<<1),SM_TM_AZTM_DEFAULT,NULL,0);
			bm43_str.deal_index++;
			bm43_str.delay_state=true;
			break;
		case 3:
			if(bm43_str.delay_time>1)
			{
				bm43_str.delay_time=0;
				bm43_str.delay_state=false;
				bm43_str.deal_index++;
			}
			break;
		case 4:
			I2C_Read_Data(SlaveAddr,GetDat,0,7);
			Register[STATUS] = GetDat[0];
			Register[SENSOR_HIGH] = GetDat[1];
			Register[SENSOR_MIDDLE] = GetDat[2];
			Register[SENSOR_LOW] = GetDat[3];
			Register[TEMP_HIGH] = GetDat[4];
			Register[TEMP_MIDDLE] = GetDat[5];
			Register[TEMP_LOW] = GetDat[6];
			bm43_str.deal_index++;
			bm43_str.delay_state=true;
			break;
		case 5:
			if(bm43_str.delay_time>1)
			{
				bm43_str.delay_time=0;
				bm43_str.delay_state=false;
				bm43_str.deal_index++;
			}
			break;
		case 6:
			I2C_Write_Data((SlaveAddr<<1),Start_NOM,NULL,0);
			bm43_str.deal_index++;
			bm43_str.delay_state=true;
			break;
		case 7:
			if(bm43_str.delay_time>1)
			{
				bm43_str.delay_time=0;
				bm43_str.delay_state=false;
				bm43_str.deal_index=0;
			}
			break;
		default:
			bm43_str.deal_index=0;
			break;
	}
}

void Calcu_BM43(uint8_t * Register,float *symbol,float *temperature)
{
	float TemperatureValue = 0.0;
	float SensorValue = 0.0;
	float Ambient = 0.0;
	float Delta_T = 0.0;	
	//float Object;
	//TemperatureValue = make32(bm43_str.Register[TEMP_HIGH], bm43_str.Register[TEMP_MIDDLE], bm43_str.Register[TEMP_LOW]);
	
	TemperatureValue = (float)(Register[TEMP_HIGH]*65536)+(float)(Register[TEMP_MIDDLE]*256)+(float)Register[TEMP_LOW];
		
	//SensorValue = make32(bm43_str.Register[SENSOR_HIGH], bm43_str.Register[SENSOR_MIDDLE], bm43_str.Register[SENSOR_LOW]);
	
	SensorValue = (float)((float)Register[SENSOR_HIGH]*(float)65536)+(float)((float)Register[SENSOR_MIDDLE]*(float)256)+(float)Register[SENSOR_LOW];
		
//	if (TemperatureValue > 8388607)//Change from 2¡¯s complement
//	  TemperatureValue = 16777216 - TemperatureValue;
//	if (SensorValue > 8388607) //Change from 2¡¯s complement
//	  SensorValue = 16777216 - SensorValue;
	
	*symbol=Ambient = (TemperatureValue / (float)pow(2, 24))*(float)125.0 - (float)40.0;
	Delta_T = (float)1000.0*(SensorValue / (float)pow(2, 24) -(float) 0.5);
	*temperature = Ambient + Delta_T;  
}

void BM43_Temp_work(void)
{
	Get_Bm43Data(bm43_str.Register);
	Calcu_BM43(bm43_str.Register,&bm43_str.Temp[0],&bm43_str.Temp[1]);
}

float Get_BM43_Temp(void)
{ 
	return bm43_str.Temp[1];
}

void bm43_interrpt(void)
{
	if(bm43_str.delay_state)bm43_str.delay_time++;
}
