#ifndef _DUI_H_
#define _DUI_H_
#include "ddl.h"

#define BUFFER_MAX  200               //缓冲区大小

typedef struct 
{
  unsigned char headPosition;        //缓冲区头部位置
  unsigned char tailPositon;         //缓冲区尾部位置
  uint16_t ringBuf[BUFFER_MAX]; //缓冲区数组
} ringBuffer_t;

void RingBuf_Write(uint16_t data,ringBuffer_t *buffer);
uint8_t RingBuf_Read(uint8_t* pData,ringBuffer_t *buffer);
#endif

