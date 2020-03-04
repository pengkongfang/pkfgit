#include "user_flash.h"
#include "flash.h"

#define FLASH_ADDR 0X7FE2

void FlashInt(void)
 {
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);
        //u32FlashTestFlag |= 0x01;
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        //u32FlashTestFlag |= 0x02;
        Flash_DisableIrq(flash_int1);
    }
      
}
 

void  Flash_init(void)
{
	 Flash_Init(FlashInt, 0);
}

void  Flash_write(Para_Str_typedef *Para_Str)
{
		Flash_SectorErase(FLASH_ADDR);
		
		Flash_WriteHalfWord(FLASH_ADDR+1,Para_Str->hight_str.down_hight); 
		Flash_WriteHalfWord(FLASH_ADDR+3,Para_Str->hight_str.top_hight); 

		Flash_WriteHalfWord(FLASH_ADDR+5,Para_Str->hight_str.set_hight[0]);
		Flash_WriteHalfWord(FLASH_ADDR+7,Para_Str->hight_str.set_hight[1]);
		Flash_WriteHalfWord(FLASH_ADDR+9,Para_Str->hight_str.set_hight[2]);
		Flash_WriteHalfWord(FLASH_ADDR+11,Para_Str->hight_str.set_hight[3]);
	
		Flash_WriteWord(FLASH_ADDR+13,Para_Str->time_str.set_Remindtime); 
	
		Flash_WriteByte(FLASH_ADDR+17,Para_Str->mode_str.Block_mode);
		Flash_WriteByte(FLASH_ADDR+18,Para_Str->mode_str.Knock_mode);
		Flash_WriteByte(FLASH_ADDR+19,Para_Str->mode_str.Remind_mode);
		Flash_WriteByte(FLASH_ADDR+20,Para_Str->mode_str.unit_mode);
}



void  Flash_read(Para_Str_typedef *Para_Str)
{
	Para_Str->hight_str.now_hight=20;
	Para_Str->hight_str.down_hight=20;
	Para_Str->hight_str.top_hight=150;
	Para_Str->hight_str.set_hight[0]=50;
	Para_Str->hight_str.set_hight[1]=50;
	Para_Str->hight_str.set_hight[2]=50;
	Para_Str->hight_str.set_hight[3]=50;
	Para_Str->time_str.set_Remindtime=15;
	Para_Str->mode_str.Block_mode=1;
	Para_Str->mode_str.Remind_mode=1;
	Para_Str->mode_str.Knock_mode=0;
	Para_Str->mode_str.unit_mode=0;
//		Para_Str->hight_str.down_hight=*((volatile uint16_t*)FLASH_ADDR+1); 
//		if(Para_Str->hight_str.down_hight==0)
//		{
//			Para_Str->hight_str.down_hight=20;
//		}
//		
//		Para_Str->hight_str.top_hight=*((volatile uint16_t*)FLASH_ADDR+3); 
//		if(Para_Str->hight_str.top_hight==0)
//		{
//			Para_Str->hight_str.top_hight=200;
//		}
//		Para_Str->hight_str.set_hight[0]=*((volatile uint16_t*)FLASH_ADDR+5);
//		Para_Str->hight_str.set_hight[1]=*((volatile uint16_t*)FLASH_ADDR+7);
//		Para_Str->hight_str.set_hight[2]=*((volatile uint16_t*)FLASH_ADDR+9);
//		Para_Str->hight_str.set_hight[3]=*((volatile uint16_t*)FLASH_ADDR+11);
//	
//		Para_Str->time_str.set_Remindtime=*((volatile uint16_t*)FLASH_ADDR+13); 
//		if(Para_Str->time_str.set_Remindtime==0)
//		{
//			Para_Str->time_str.set_Remindtime=120;
//		}
//		Para_Str->mode_str.Block_mode=*((volatile uint8_t*)FLASH_ADDR+17);
//		Para_Str->mode_str.Knock_mode=*((volatile uint8_t*)FLASH_ADDR+18);
//		Para_Str->mode_str.Remind_mode=*((volatile uint8_t*)FLASH_ADDR+19);
//		Para_Str->mode_str.unit_mode=*((volatile uint8_t*)FLASH_ADDR+20);
}

