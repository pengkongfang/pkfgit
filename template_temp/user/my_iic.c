#include "my_iic.h"

#include "ddl.h"
#include "gpio.h"
#define IIC_SCL_HI     Gpio_SetIO(1,4,1);							    //PA15输出高 															//SCL 高
#define IIC_SCL_LOW    Gpio_SetIO(1,4,0);	 									  //PA15 输出低														//SCL 低

#define IIC_SDA_HI     Gpio_SetIO(1,5,1);									//SDA	 
#define IIC_SDA_LOW    Gpio_SetIO(1,5,0);								//SDA	 
#define READ_SDA   		 Gpio_GetIO(1,5)  															//输入SDA 

/**************************************************************
函数名称：	SDA设置成输入
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void SDA_IN(void)
{
    Gpio_InitIOExt(1,5,GpioDirIn,FALSE,FALSE,TRUE,FALSE);   
    
}
/**************************************************************
函数名称：	SDA设置成输出
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void SDA_OUT(void)
{
    Gpio_InitIOExt(1,5,GpioDirOut,FALSE,FALSE,TRUE,FALSE);
}

/**************************************************************
函数名称：	ICC 起始信号
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void IIC_Start(void)
{
    SDA_OUT();     																//sda线输出
    IIC_SDA_HI
    IIC_SCL_HI
    delay1us(4);
    IIC_SDA_LOW																		//START:when CLK is high,DATA change form high to low
    delay1us(4);
    IIC_SCL_LOW																		//钳住I2C总线，准备发送或接收数据
}
/**************************************************************
函数名称：	ICC 停止信号
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void IIC_Stop(void)
{
    SDA_OUT();																		//sda线输出
    IIC_SCL_LOW
    IIC_SDA_LOW																	//STOP:when CLK is high DATA change form low to high
    delay1us(4);
    IIC_SCL_HI
    IIC_SDA_HI																	//发送I2C总线结束信号
    delay1us(4);
}

/**************************************************************
函数名称：	ICC 等待应答信号
函数说明：
函出入口参数：返回值：1，接收应答失败    0，接收应答成功
更改备注：
**************************************************************/
uint8_t IIC_Wait_Ack(void)
{
    uint8_t ucErrTime=0;
    SDA_IN();      									//SDA设置为输入
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
    IIC_SCL_LOW											//时钟输出0
    return 0;
}
/**************************************************************
函数名称：	ICC 产生ACK信号
函数说明：
函出入口参数：
更改备注：
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
函数名称：	ICC 不产生ACK信号
函数说明：
函出入口参数：
更改备注：
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
函数名称：	ICC 发送一个字节
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SDA_OUT();
    IIC_SCL_LOW												//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        if((txd&0x80)>>7)
            IIC_SDA_HI
            else
                IIC_SDA_LOW
                txd<<=1;
        delay1us(2);   									//对TEA5767这三个延时都是必须的
        IIC_SCL_HI
        delay1us(2);
        IIC_SCL_LOW
        delay1us(2);
    }
}

/**************************************************************
函数名称：	ICC 读一个字节
函数说明：
函出入口参数：返回值  读取的数值	 ack=1时 发送ACK，ack=0，发送nACK
更改备注：
**************************************************************/
uint8_t IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    SDA_IN();													//SDA设置为输入
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
        IIC_NAck();									//发送nACK
    else
        IIC_Ack(); 									//发送ACK
    return receive;
}



/**************************************************************
函数名称：	AT24C02从一个地址读取一个数据
函数说明：
函出入口参数：
更改备注：
**************************************************************/
uint8_t AT24C02_ReadByte(uint16_t ReadAddr,uint8_t * buf,uint8_t reg_addr,uint8_t len)
{
    uint8_t temp=0;
    IIC_Start();
    IIC_Send_Byte(ReadAddr|1);   							//发送器件地址0XA0,写数据
    IIC_Wait_Ack();
		for(uint8_t i=0;i<len-1;i++)
	  {
      temp=*buf++=IIC_Read_Byte(1);
		}
		temp=*buf++=IIC_Read_Byte(0);
    IIC_Stop();																						  //产生一个停止条件
    return temp;
}
/**************************************************************
函数名称：	AT24C02往一个地址写入一个数据
函数说明：
函出入口参数：
更改备注：
**************************************************************/
void WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite)
{
    IIC_Start();
    IIC_Send_Byte(WriteAddr);   						//发送器件地址0XA0,写数据
    IIC_Wait_Ack();
    IIC_Send_Byte(DataToWrite);   											  //发送低地址
    IIC_Wait_Ack();
    IIC_Stop();																						  //产生一个停止条件
}





