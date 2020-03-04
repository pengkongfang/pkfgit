#ifndef _USER_SEVER_H_
#define _USER_SEVER_H_

#include "ddl.h"
#include "stdbool.h"
#define MAX_HIGHT  (Para_str.mode_str.unit_mode)?(1800*2.54):(1800)
#define MIN_HIGHT  (Para_str.mode_str.unit_mode)?(200*2.54):(200)


enum PARA_ENUM{
	HIGHT_NOW =1,
	HIGHT_TOP =2,
	HIGHT_DWON ,
	HIGHT_SET1 ,
	HIGHT_SET2 ,
	HIGHT_SET3 ,
	HIGHT_SET4 ,
	TIME_SETREMINDER ,
	TIME_NOWREMINDER ,
	MODE_REMINDER,
	MODE_WILLBLOCK,
	MODE_BLOCK,
	MODE_KNOCK,
	MODE_UNIT,
	MODE_VER,
	ACTIVE_SULT,
	PROBLEM,
	RELEASE,
	HIGHT_LIE,
};


typedef struct 
{
	struct HIGHT
	{
		uint16_t lie_hight;
		uint16_t now_hight;
		uint16_t top_hight;
		uint16_t down_hight;
		uint16_t set_hight[4];
		bool TopSetFlag;
		bool DownSetFlag;
		
	}hight_str;
	struct TIME
	{
		uint16_t set_Remindtime;
		uint16_t now_Remindtime;
	}time_str;
	struct MODE
	{
		uint8_t Remind_mode;
		uint8_t Block_mode;
		uint8_t WillBlock_mode;
		uint8_t Knock_mode;
		uint8_t unit_mode;
	}mode_str;
	uint8_t problem;
	uint8_t version;
	uint8_t Set_Result;
	uint8_t FirstPower;
	uint8_t Relaseend;
	bool lie;
}Para_Str_typedef;
extern Para_Str_typedef Para_str;
uint32_t Para_read(enum PARA_ENUM dat);
void Para_write(enum PARA_ENUM dat ,uint32_t value,uint8_t istrue);
void user_init(void);
void user_work(void);







#endif

