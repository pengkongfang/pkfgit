#include "dui.h"

void RingBuf_Write(uint16_t data,ringBuffer_t *buffer)
{
  buffer->ringBuf[buffer->tailPositon]=data;     //��β��׷��
  if(++buffer->tailPositon>=BUFFER_MAX)         //β�ڵ�ƫ��
    buffer->tailPositon=0;                      //����������󳤶� ���� �γɻ��ζ���
  if(buffer->tailPositon == buffer->headPosition)//���β���ڵ�׷��ͷ���ڵ㣬���޸�ͷ�ڵ�ƫ��λ�ö�����������
	if(++buffer->headPosition>=BUFFER_MAX)
		buffer->headPosition=0;
}
 

uint8_t RingBuf_Read(uint8_t* pData,ringBuffer_t *buffer)
{
  if(buffer->headPosition == buffer->tailPositon)    //���ͷβ�Ӵ���ʾ������Ϊ��
	{
					return 1;   //����1�����λ������ǿյ�
	}
  else
  {
    *pData=buffer->ringBuf[buffer->headPosition];    //����������ǿ���ȡͷ�ڵ�ֵ��ƫ��ͷ�ڵ�
    if(++buffer->headPosition>=BUFFER_MAX)
      buffer->headPosition=0;
    return 0;     //����0����ʾ��ȡ���ݳɹ�
  }
}

