#include "user_uart.h"

uart_str Uart_0;

void RxIntCallback(void)
{
    RingBuf_Write(Uart_ReceiveData(UARTCH0),&Uart_0.uart_fifo);
}


void ErrIntCallback(void)
{
  
}


void uart_init(void)
{
		uint16_t timer=0;
    uint32_t pclk=0;
	
    stc_uart_config_t  stcConfig;
    stc_uart_irq_cb_t stcUartIrqCb;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_config_t stcBaud;
    stc_bt_config_t stcBtConfig;
    

    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    DDL_ZERO_STRUCT(stcBtConfig);

    
    Gpio_InitIOExt(1,4,GpioDirOut,TRUE,FALSE,FALSE,FALSE);   
    Gpio_InitIOExt(1,5,GpioDirOut,TRUE,FALSE,FALSE,FALSE); 
    
    //ͨ���˿�����
    Gpio_SetFunc_UART0TX_P14();
    Gpio_SetFunc_UART0RX_P15();

    //����ʱ��ʹ��
    Clk_SetPeripheralGate(ClkPeripheralBt,TRUE);//ģʽ0/2���Բ�ʹ��
    Clk_SetPeripheralGate(ClkPeripheralUart0,TRUE);



    stcUartIrqCb.pfnRxIrqCb = RxIntCallback;
    stcUartIrqCb.pfnTxIrqCb = NULL;
    stcUartIrqCb.pfnRxErrIrqCb = ErrIntCallback;
    stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
  

    stcConfig.enRunMode = UartMode3;//��������Ĵ˴���ת��4��ģʽ����
   

    stcMulti.enMulti_mode = UartNormal;//��������Ĵ˴���ת��������ģʽ��mode2/3���ж�����ģʽ

    stcConfig.pstcMultiMode = &stcMulti;

    stcBaud.bDbaud = 0u;//˫�������ʹ���
    stcBaud.u32Baud = 9600u;//���²�����λ��
    stcBaud.u8Mode = UartMode3; //���㲨������Ҫģʽ����
    pclk = Clk_GetPClkFreq();
    timer=Uart_SetBaudRate(UARTCH0,pclk,&stcBaud);

    stcBtConfig.enMD = BtMode2;
    stcBtConfig.enCT = BtTimer;
    Bt_Init(TIM1, &stcBtConfig);//����basetimer1���ú�������������
    Bt_ARRSet(TIM1,timer);
    Bt_Cnt16Set(TIM1,timer);
    Bt_Run(TIM1);

    Uart_Init(UARTCH0, &stcConfig);
    Uart_EnableIrq(UARTCH0,UartRxIrq);
    Uart_ClrStatus(UARTCH0,UartRxFull);
    Uart_EnableFunc(UARTCH0,UartRx);
		
}


uint8_t uart_read(uint8_t* pData)
{
	uint8_t *dat=NULL;
	if(0 == RingBuf_Read(dat,&Uart_0.uart_fifo)) 
	{
		*pData=*dat;
		return 1;
	}
	else
		return 0;
}

void uart_write(uint8_t * buff , uint8_t length)
{
	for(uint8_t i=0;i<length;i++)
	{
		Uart_SendData(UARTCH0,buff[i]);
	}
}


