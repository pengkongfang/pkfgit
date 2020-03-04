#include "uart_sever.h"





/***********************************************************************************************************
* ��������: PackageSendData(u8* data, u8 len)()
* �������: data Ҫ��װ�������׵�ַ��len�����ݳ��� 
* ����ֵ  : ��
* ��    ��: ��װҪ���͵�����
************************************************************************************************************/
void PackageSendData(uint8_t* data, uint8_t* len)
{
  uint16_t crc;
  uint8_t i,j = 0;
  uint8_t temp[9],temp1[9];
  
  memset(&temp[0], 0, 9); 
  memset(&temp1[0], 0, 9);
  temp[1] = *len+4;
  temp[2] = 0x02;     //������ID  
  memcpy(&temp[3], data, *len);
  crc = CRC16MODBUS(&temp[1], *len+2);
  memcpy(&temp[*len+3],&crc, 2);
  
  for(i = 3; i<*len+5+j; )
  {
    if((temp[i] == UART_START)||(temp[i] == UART_END))//���������������к�֡ͷ��֡βһ�������������ת���
    {
      memcpy(&temp1[0], &temp[i], *len+5+j-i);
      temp[i] = 0x5C; //����ת��� ��\'
      memcpy(&temp[i+1], &temp1[0], *len+5+j-i);
      i = i+2;
      j++;
    }
    else
      i++;
  }
  temp[0] = UART_START;  
  temp[*len+5+j] = UART_END;
  memcpy(data, &temp[0], *len+6+j);
  *len = *len+6+j;
}

void Uart_deal_message(uart_str *uart)
{
	uint8_t KeyValue;
	uint8_t count = 0;
	switch(uart->datbuf[0])
	{
			case GetKeyValue: //����Ҫ���ȡ��ǰ����ֵ
				//OverTimeCnt = 0;
				//OverTimeFlag = 0;
				//ResetMainBoardCnt = 0;
				//ResetMainBoardFlag = 0;
				uart->txbuf[0] = (KeyValue&0x00ff);
				uart->txbuf[1] = (KeyValue&0xff00)>>8;
				count = 2;
				PackageSendData(&uart->txbuf[0], &count);
				uart->write(&uart->txbuf[0],count);
				break;
			case DisplayLed: //����Ҫ����ʾLED
				//OverTimeCnt = 0;
				//OverTimeFlag = 0;
				//ResetMainBoardCnt = 0;
				//ResetMainBoardFlag = 0;
//				memcpy(&Dis_Char[0], &ReceiveBuffer[2], 3);
//				if((Dis_Char[0]==0)&&(Dis_Char[1]==0)&&(Dis_Char[2]==0))
//				{
//					//LED2_OFF;
//				}
//				else
//				{
//					//DisplayFlag = ON;
//					//LED2_ON;
//				}
				break;
				
			case MainBoardPowerOff: //����Ҫ��Ͽ������Դ
				//OverTimeCnt = 0;
				//OverTimeFlag = 0;
				//ResetMainBoardCnt = 0;
			 // ResetMainBoardFlag = 0;
			
			
//            MainBoard_PowerOff;
//            DisplayFlag = OFF;
				break;
				
			case TurnOnBuzzer: //����Ҫ����������
				//OverTimeCnt = 0;
				//OverTimeFlag = 0;
				//ResetMainBoardCnt = 0;
				//ResetMainBoardFlag = 0;
//				BUZZER_ON;
				break;
				
			case TurnOffBuzzer: //����Ҫ��رշ�����
//				BUZZER_OFF;
				break; 
			case SoftVersionDisplay:
//				Dis_Char[0] = SoftVersionChar1;
//				Dis_Char[1] = SoftVersionChar2;
//				Dis_Char[2] = SoftVersionChar3;
				break;
			case DisSingleLED://0x17
				if(uart->datbuf[2]==0)
				{
					//LED2_OFF;
				}
				else
				{
					//LED2_ON;
				}
				break;                          
	}
}


void Uart_parse(uart_str *uart)
{
	uint8_t dat=0;
	uint8_t ERROR_STATE=0;
	uint16_t Crc_cal=0;
	static char Crc_get[2]={0,0};	
	static uint8_t Uart_Index;
	static uint8_t Uart_lenth;
	static uint8_t Crc_Index;
	if(uart->read(&dat)==false)
	{
		ERROR_STATE=1;
		goto err;
	}
	uart->rxbuf[uart->rx_length++]=dat;

	switch(Uart_Index)
	{
		case U_IDLE:
			if(dat==UART_START)
			Uart_Index=U_LENTH;
			break;
		case U_LENTH:
			if(dat==UART_SERISE);
			else if(dat>3&&dat<10)
			{
				Uart_lenth=dat;
				Uart_Index=U_CMD;
			}
			else
			{
					ERROR_STATE=1;
					goto err;
			}
			break;
		case U_CMD:
			if(dat==UART_SERISE);
			else
			{
				if(uart->rx_length==Uart_lenth-1)
				{
					Crc_Index=0;
					Uart_Index=U_CRC;
				}
				else
				{
					uart->datbuf[uart->rxdat_length++]=dat;
				}
			}
			break;
		case U_CRC:
			Crc_get[Crc_Index++]=dat;
			if(Crc_Index==2)
			{
				Crc_cal=CRC16MODBUS(uart->rxbuf,uart->rxdat_length);
				if(Crc_cal==(uint16_t)(Crc_get[0]<<8||Crc_get[1]))
					Uart_Index=U_END;
				else
				{
					ERROR_STATE=1;
					goto err;
				}
			}
			break;
		case U_END:
			if(dat==UART_END)
			{
				Uart_deal_message(uart);
				uart->rx_length=0;
				Uart_Index=U_IDLE;
			}
			else
			{
				ERROR_STATE=1;
				goto err;
			}
			break;
		default:
			break;
	}
	
	err:
		if(ERROR_STATE)
		{
			uart->rx_length=0;
			Uart_Index=U_IDLE;
		}
}

