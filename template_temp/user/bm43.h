#ifndef _BM43_H_
#define _BM43_H_

#include "ddl.h"

//General Sensor Support Functions
//RAW SENSOR Reading 24bit format
#define SM_DEFAULT 0xA0 //0x0000
#define SM_USERSET 0xA1 //User Set
#define SM_AZSM_DEFAULT 0xA2 //0x0000/ ppppppppp
#define SM_AZSM_USERSET 0xA3 //User Set
#define TM_DEFAULT 0xA4 //0x0000
#define TM_USERSET 0xA5 //User Set
#define TM_AZTM_DEFAULT 0xA6 //0x0000
#define TM_AZTM_USERSET 0xA7 //User Set
#define SM_TM_AZTM_DEFAULT 0xAA
// Command List
#define Start_NOM 0xA8
#define Start_CM 0xA9
#define Measure 0xAA
#define MeasureCyclic 0xAB
#define Over2Measure 0xAC
#define Over4Measure 0xAD
#define Over8Measure 0xAE
#define Over16Measure 0xAF
#define SET_SM_Config1 0xB0
#define SET_SM_Config2 0xB1
#define STOP_CYC 0xBF
#define NOP 0xF0
// Not Assigned 8bit Registers
#define STATUS 0x00
#define SENSOR_HIGH 0x01
#define SENSOR_MIDDLE 0x02
#define SENSOR_LOW 0x03
#define TEMP_HIGH 0x04
#define TEMP_MIDDLE 0x05
#define TEMP_LOW 0x06
#define COMMAND 0x07
#define COMMAND_DAT1 0x08
#define COMMAND_DAT2 0x09
#define READ_REGISTER 0x0A
#define READ_MEMDAT1 0x0B
#define READ_MEMDAT2 0x0C




struct bm43_Str{
	uint8_t deal_index;
	uint8_t delay_time;
	uint8_t delay_state;
	uint8_t Register[7];
	float Temp[2];
};


float Get_BM43_Temp(void);
void BM43_Temp_work(void);
void bm43_interrpt(void);
extern struct bm43_Str bm43_str;

#endif

