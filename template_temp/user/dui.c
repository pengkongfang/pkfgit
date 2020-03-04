#include "dui.h"

void RingBuf_Write(uint16_t data,ringBuffer_t *buffer)
{
  buffer->ringBuf[buffer->tailPositon]=data;     //从尾部追加
  if(++buffer->tailPositon>=BUFFER_MAX)         //尾节点偏移
    buffer->tailPositon=0;                      //大于数组最大长度 归零 形成环形队列
  if(buffer->tailPositon == buffer->headPosition)//如果尾部节点追到头部节点，则修改头节点偏移位置丢弃早期数据
	if(++buffer->headPosition>=BUFFER_MAX)
		buffer->headPosition=0;
}
 

uint8_t RingBuf_Read(uint8_t* pData,ringBuffer_t *buffer)
{
  if(buffer->headPosition == buffer->tailPositon)    //如果头尾接触表示缓冲区为空
	{
					return 1;   //返回1，环形缓冲区是空的
	}
  else
  {
    *pData=buffer->ringBuf[buffer->headPosition];    //如果缓冲区非空则取头节点值并偏移头节点
    if(++buffer->headPosition>=BUFFER_MAX)
      buffer->headPosition=0;
    return 0;     //返回0，表示读取数据成功
  }
}

