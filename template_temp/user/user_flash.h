#ifndef _USER_FLASH_H
#define _USER_FLASH_H

#include "ddl.h"
#include "user_sever.h"

typedef struct 
{
	void (*init)(void);
	void (*write)(Para_Str_typedef *Para_Str);
	void (*read)(Para_Str_typedef *Para_Str);
}Flash_ops_typedef;


extern Flash_ops_typedef Flash_ops_str;
void flash_register(Flash_ops_typedef *flash_ops_str);


#endif

