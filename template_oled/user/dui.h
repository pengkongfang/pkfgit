#ifndef _DUI_H_
#define _DUI_H_
#include "ddl.h"

#define BUFFER_MAX  200               //��������С

typedef struct 
{
  unsigned char headPosition;        //������ͷ��λ��
  unsigned char tailPositon;         //������β��λ��
  uint16_t ringBuf[BUFFER_MAX]; //����������
} ringBuffer_t;

void RingBuf_Write(uint16_t data,ringBuffer_t *buffer);
uint8_t RingBuf_Read(uint8_t* pData,ringBuffer_t *buffer);
#endif

