#ifndef _MY_IIC_
#define _MY_IIC_
#include "ddl.h"

uint8_t AT24C02_ReadByte(uint16_t ReadAddr,uint8_t * buf,uint8_t reg_addr,uint8_t len);
void WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);

#endif

