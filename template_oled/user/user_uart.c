#include "user_uart.h"
#include "uart_sever.h"

uart_str Uart_1;
uint8_t u8RxData;
uint16_t u16Rxdata;
uint8_t rx_index;
uint8_t rx_lenth;
//UART1�ж�
void Uart1_IRQHandler(void)
{
    if(Uart_GetStatus(M0P_UART1, UartRC))
    {
        Uart_ClrStatus(M0P_UART1, UartRC);              //����ж�״̬λ
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
////UART1�ж�
//void Uart1_IRQHandler(void)
//{
//    if(Uart_GetStatus(M0P_UART1, UartRC))
//    {
//        Uart_ClrStatus(M0P_UART1, UartRC);              //����ж�״̬λ
//				u8RxData=Uart_ReceiveData(M0P_UART1);  
//				u16Rxdata=u16Rxdata<<8|u8RxData;
//				RingBuf_Write(u8RxData,&Uart_1.uart_fifo);
//				if(u8RxData==UART_END&&u16Rxdata!=0x5c9d)
//				Uart_1.wait_deal_num++;
//    }
//}
//����ģ������
void App_UartCfg(void)
{
    stc_uart_cfg_t  stcCfg;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcBaud);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); ///<ʹ��UART1����ʱ���ſؿ���

    ///<UART Init
    stcCfg.enRunMode        = UartMskMode1;                 ///<ģʽ3
    stcCfg.enStopBit        = UartMsk1bit;                  ///<1bitֹͣλ
    stcCfg.enMmdorCk        = UartMskDataOrAddr;            ///<���ģʽʱ
    stcCfg.stcBaud.u32Baud  = 9600;                         ///<������9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;              ///<ͨ��������Ƶ����
    stcCfg.stcBaud.u32Pclk  = Sysctrl_GetPClkFreq();        ///</<�������ʱ�ӣ�PCLK��Ƶ��ֵ
    Uart_Init(M0P_UART1, &stcCfg);                          ///<���ڳ�ʼ��

    ///<UART�ж�ʹ��
    Uart_ClrStatus(M0P_UART1,UartRC);                       ///<���������
    Uart_ClrStatus(M0P_UART1,UartTC);                       ///<���������
    Uart_EnableIrq(M0P_UART1,UartRxIrq);                    ///<ʹ�ܴ��ڽ����ж�
    EnableNvic(UART1_IRQn, IrqLevel3, TRUE);              ///<ϵͳ�ж�ʹ��

}

//������������
void App_PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;

    DDL_ZERO_STRUCT(stcGpioCfg);

    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  ///<ʹ��GPIO����ʱ���ſؿ���

    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin2,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1);             ///<����PA02 ΪUART1 TX
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin3,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1);             ///<����PA03 ΪUART1 RX
}

void uart_init(void)
{
		//UART�˿ڳ�ʼ��
    App_PortInit();
    //����ģ������
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

