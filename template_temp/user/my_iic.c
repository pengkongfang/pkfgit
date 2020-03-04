#include "my_iic.h"

#include "ddl.h"
#include "gpio.h"
#define IIC_SCL_HI     Gpio_SetIO(1,4,1);							    //PA15����� 															//SCL ��
#define IIC_SCL_LOW    Gpio_SetIO(1,4,0);	 									  //PA15 �����														//SCL ��

#define IIC_SDA_HI     Gpio_SetIO(1,5,1);									//SDA	 
#define IIC_SDA_LOW    Gpio_SetIO(1,5,0);								//SDA	 
#define READ_SDA   		 Gpio_GetIO(1,5)  															//����SDA 

/**************************************************************
�������ƣ�	SDA���ó�����
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void SDA_IN(void)
{
    Gpio_InitIOExt(1,5,GpioDirIn,FALSE,FALSE,TRUE,FALSE);   
    
}
/**************************************************************
�������ƣ�	SDA���ó����
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void SDA_OUT(void)
{
    Gpio_InitIOExt(1,5,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
}

/**************************************************************
�������ƣ�	ICC ��ʼ�ź�
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void IIC_Start(void)
{
    SDA_OUT();     																//sda�����
    IIC_SDA_HI
    IIC_SCL_HI
    delay1us(4);
    IIC_SDA_LOW																		//START:when CLK is high,DATA change form high to low
    delay1us(4);
    IIC_SCL_LOW																		//ǯסI2C���ߣ�׼�����ͻ��������
}
/**************************************************************
�������ƣ�	ICC ֹͣ�ź�
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void IIC_Stop(void)
{
    SDA_OUT();																		//sda�����
    IIC_SCL_LOW
    IIC_SDA_LOW																	//STOP:when CLK is high DATA change form low to high
    delay1us(4);
    IIC_SCL_HI
    IIC_SDA_HI																	//����I2C���߽����ź�
    delay1us(4);
}

/**************************************************************
�������ƣ�	ICC �ȴ�Ӧ���ź�
����˵����
������ڲ���������ֵ��1������Ӧ��ʧ��    0������Ӧ��ɹ�
���ı�ע��
**************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      									//SDA����Ϊ����
    IIC_SDA_HI delay1us(1);
    IIC_SCL_HI delay1us(1);
    while(READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL_LOW											//ʱ�����0
    return 0;
}
/**************************************************************
�������ƣ�	ICC ����ACK�ź�
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void IIC_Ack(void)
{
    IIC_SCL_LOW
    SDA_OUT();
    IIC_SDA_LOW
    delay1us(2);
    IIC_SCL_HI
    delay1us(2);
    IIC_SCL_LOW
}
/**************************************************************
�������ƣ�	ICC ������ACK�ź�
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void IIC_NAck(void)
{
    IIC_SCL_LOW
    SDA_OUT();
    IIC_SDA_HI
    delay1us(2);
    IIC_SCL_HI
    delay1us(2);
    IIC_SCL_LOW
}

/**************************************************************
�������ƣ�	ICC ����һ���ֽ�
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL_LOW												//����ʱ�ӿ�ʼ���ݴ���
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
            IIC_SDA_HI
            else
                IIC_SDA_LOW
                txd<<=1;
        delay1us(2);   									//��TEA5767��������ʱ���Ǳ����
        IIC_SCL_HI
        delay1us(2);
        IIC_SCL_LOW
        delay1us(2);
    }
}

/**************************************************************
�������ƣ�	ICC ��һ���ֽ�
����˵����
������ڲ���������ֵ  ��ȡ����ֵ	 ack=1ʱ ����ACK��ack=0������nACK
���ı�ע��
**************************************************************/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();													//SDA����Ϊ����
    for(i=0; i<8; i++ )
    {
        IIC_SCL_LOW
        delay1us(2);
        IIC_SCL_HI
        receive<<=1;
        if(READ_SDA) receive++;
        delay1us(1);
    }
    if (!ack)
        IIC_NAck();									//����nACK
    else
        IIC_Ack(); 									//����ACK
    return receive;
}



/**************************************************************
�������ƣ�	AT24C02��һ����ַ��ȡһ������
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
uint8_t AT24C02_ReadByte(uint16_t ReadAddr,uint8_t * buf,uint8_t reg_addr,uint8_t len)
{
    uint8_t temp=0;
    IIC_Start();
    IIC_Send_Byte(ReadAddr|1);   							//����������ַ0XA0,д����
    IIC_Wait_Ack();
		for(uint8_t i=0;i<len-1;i++)
	  {
      temp=*buf++=IIC_Read_Byte(1);
		}
		temp=*buf++=IIC_Read_Byte(0);
    IIC_Stop();																						  //����һ��ֹͣ����
    return temp;
}
/**************************************************************
�������ƣ�	AT24C02��һ����ַд��һ������
����˵����
������ڲ�����
���ı�ע��
**************************************************************/
void WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
    IIC_Start();
    IIC_Send_Byte(WriteAddr);   						//����������ַ0XA0,д����
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);   											  //���͵͵�ַ
    IIC_Wait_Ack();
    IIC_Stop();																						  //����һ��ֹͣ����
}





