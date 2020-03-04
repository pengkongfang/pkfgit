#include "user_uart.h"
#include "uart_sever.h"

uart_str Uart_1;
uint8_t u8RxData;
uint16_t u16Rxdata;
uint8_t rx_index;
uint8_t rx_lenth;
//UART1中断
void Uart1_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);              //清除中断状态位
				u8RxData=Uart_ReceiveData(M0P_UART1);  
				RingBuf_Write(u8RxData,&Uart_1.uart_fifo);
				switch(rx_index)
				{
					case 0:
							if(u8RxData==UART_START)
								rx_index=1;
						break;
					case 1:
								rx_lenth=u8RxData;
								if(rx_lenth<=23)
								rx_index=2;
								else
								rx_index=0;
						break;
					case 2:
							rx_lenth--;
							if(rx_lenth==false)
							{
								if(u8RxData==UART_END)
								Uart_1.wait_deal_num++;
								
								rx_index=0;
							}
						break;
				}
    }
}
////UART1中断
//void Uart1_IRQHandler(void)
//{
//    if(Uart_GetStatus(M0P_UART1, UartRC))
//    {
//        Uart_ClrStatus(M0P_UART1, UartRC);              //清除中断状态位
//				u8RxData=Uart_ReceiveData(M0P_UART1);  
//				u16Rxdata=u16Rxdata<<8|u8RxData;
//				RingBuf_Write(u8RxData,&Uart_1.uart_fifo);
//				if(u8RxData==UART_END&&u16Rxdata!=0x5c9d)
//				Uart_1.wait_deal_num++;
//    }
//}
//串口模块配置
void App_UartCfg(void)
{
    stc_uart_cfg_t  stcCfg;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcBaud);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); ///<使能UART1外设时钟门控开关

    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;                 ///<模式3
    stcCfg.enStopBit        = UartMsk1bit;                  ///<1bit停止位
    stcCfg.enMmdorCk        = UartMskDataOrAddr;            ///<多机模式时
    stcCfg.stcBaud.u32Baud  = 9600;                         ///<波特率9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;              ///<通道采样分频配置
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq();        ///</<获得外设时钟（PCLK）频率值
    Uart_Init(M0P_UART1, &stcCfg);                          ///<串口初始化

    ///<UART中断使能
    Uart_ClrStatus(M0P_UART1,UartRC);                       ///<清接收请求
    Uart_ClrStatus(M0P_UART1,UartTC);                       ///<清接收请求
    Uart_EnableIrq(M0P_UART1,UartRxIrq);                    ///<使能串口接收中断
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);              ///<系统中断使能

}

//串口引脚配置
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  ///<使能GPIO外设时钟门控开关

    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin2,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1);             ///<配置PA02 为UART1 TX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin3,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1);             ///<配置PA03 为UART1 RX
}

void uart_init(void)
{
		//UART端口初始化
    App_PortInit();
    //串口模块配置
    App_UartCfg();

}


uint8_t uart_read(uint8_t* pData)
{
	uint8_t dat=NULL;
	if(0 == RingBuf_Read(&dat,&Uart_1.uart_fifo)) 
	{
		*pData=dat;
		return 1;
	}
	else
		return 0;
}
 
void uart_write(uint8_t * buff , uint8_t length)
{
	for(uint8_t i=0;i<length;i++)
	{
		Uart_SendDataPoll(M0P_UART1,buff[i]);
	}
}

