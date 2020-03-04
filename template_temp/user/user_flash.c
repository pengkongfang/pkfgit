#include "user_flash.h"
#include "flash.h"

#define FLASH_ADDR 0X7000

Flash_ops_typedef Flash_ops_str;
static volatile uint32_t u32FlashTestFlag   = 0;
void FlashInt(void)
 {
    if (TRUE == Flash_GetIntFlag(flash_int0))
    {
        Flash_ClearIntFlag(flash_int0);
        u32FlashTestFlag |= 0x01;
        Flash_DisableIrq(flash_int0);
    }
    if (TRUE == Flash_GetIntFlag(flash_int1))
    {
        Flash_ClearIntFlag(flash_int1);
        u32FlashTestFlag |= 0x02;
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

int Flash_ReadByte(uint32_t ADD)
{  
	return(*((volatile uint8_t*)ADD));
}

int Flash_ReadHalfWord(uint32_t ADD)
{  
	return(*((volatile uint16_t*)ADD));
}

int Flash_ReadWord(uint32_t ADD)
{  
	return(*((volatile uint32_t*)ADD));
}


void  Flash_read(Para_Str_typedef *Para_Str)
{
		Para_Str->hight_str.down_hight=Flash_ReadHalfWord(FLASH_ADDR+1); 
		Para_Str->hight_str.top_hight=Flash_ReadHalfWord(FLASH_ADDR+3); 

		Para_Str->hight_str.set_hight[0]=Flash_ReadHalfWord(FLASH_ADDR+5);
		Para_Str->hight_str.set_hight[1]=Flash_ReadHalfWord(FLASH_ADDR+7);
		Para_Str->hight_str.set_hight[2]=Flash_ReadHalfWord(FLASH_ADDR+9);
		Para_Str->hight_str.set_hight[3]=Flash_ReadHalfWord(FLASH_ADDR+11);
	
		Para_Str->time_str.set_Remindtime=Flash_ReadWord(FLASH_ADDR+13); 
	
		Para_Str->mode_str.Block_mode=Flash_ReadByte(FLASH_ADDR+17);
		Para_Str->mode_str.Knock_mode=Flash_ReadByte(FLASH_ADDR+18);
		Para_Str->mode_str.Remind_mode=Flash_ReadByte(FLASH_ADDR+19);
		Para_Str->mode_str.unit_mode=Flash_ReadByte(FLASH_ADDR+20);
}




void flash_register(Flash_ops_typedef *flash_ops_str)
{
	flash_ops_str->init=Flash_init;
	flash_ops_str->read=Flash_read;
	flash_ops_str->write=Flash_write;
	flash_ops_str->init();
}
