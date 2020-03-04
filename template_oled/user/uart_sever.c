#include "uart_sever.h"
#include "user_Sever.h"
#include "lcd_drive.h"
#include "lcd12864.h"
#include "misc.h"
#include "beep.h"


enum{
	U_IDLE=0,
	U_LENTH=1,
	U_CMD=2,
	U_CRC=3,
	U_END=4,
}UART_STATE;

///***********************************************************************************************************
//* 函数名称: PackageSendData(u8* data, u8 len)()
//* 输入参数: data 要封装的数据首地址，len，数据长度 
//* 返回值  : 无
//* 功    能: 封装要发送的数据
//************************************************************************************************************/
//void PackageSendData(uint8_t* data, uint8_t* len)
//{
//  uint16_t crc;
//  uint8_t i,j = 0;
//  uint8_t temp[9],temp1[9];
//  
//  memset(&temp[0], 0, 9); 
//  memset(&temp1[0], 0, 9);
//  temp[1] = *len+3;
//  memcpy(&temp[2], data, *len);
//  crc = CRC16MODBUS(&temp[1], *len+1);
//  memcpy(&temp[*len+3],&crc, 2);
//  
//  for(i = 3; i<*len+5+j; )
//  {
//    if((temp[i] == UART_START)||(temp[i] == UART_END))//如果传输的数据中有和帧头或帧尾一样的数据则插入转义符
//    {
//      memcpy(&temp1[0], &temp[i], *len+5+j-i);
//      temp[i] = 0x5C; //加入转义符 ‘\'
//      memcpy(&temp[i+1], &temp1[0], *len+5+j-i);
//      i = i+2;
//      j++;
//    }
//    else
//      i++;
//  }
//  temp[0] = UART_START;  
//  temp[*len+5+j] = UART_END;
//  memcpy(data, &temp[0], *len+6+j);
//  *len = *len+6+j;
//}


void make_uartSend_Data(uint8_t* data, uint8_t len)
{
	uint16_t crc;
	//uint8_t tempbuf;
	uint8_t temp[16]={UART_START,len+3};
	memcpy(temp+2,data,len);
	crc = CRC16MODBUS(&temp[1], len+1);
	temp[len+2]=crc>>8;
	temp[len+3]=crc&0xff;
	//memcpy(&temp[len+2],&crc, 2);
	temp[len+4]=UART_END;
	uart_write(temp,len+5);
}

void Uart_Send_Forward(uint8_t Forward)
{
	uint8_t Uart_Up_Char[2]={0X5A,Forward};
	make_uartSend_Data(Uart_Up_Char,2);
}
const char Uart_HightRead_Char[1]={UART_GETHIGHT};

void Uart_Send_ReadHight(void)
{
	make_uartSend_Data((uint8_t *)Uart_HightRead_Char,1);
}
const char Uart_AutoBalence_Char[1]={UART_IBALENCE};
void Uart_Send_AutoBalence(void)
{
	Uart_1.uart_want_get=UART_IBALENCE;
	make_uartSend_Data((uint8_t *)Uart_AutoBalence_Char,1);
}
const char Uart_CheckBalence_Char[1]={UART_CBALENCE};
void Uart_Send_CheckBalence(void)
{
	Uart_1.uart_want_get=UART_CBALENCE;
	make_uartSend_Data((uint8_t *)Uart_CheckBalence_Char,1);
}
const char Uart_CheckBlock_Char[1]={UART_CBLOCK};
void Uart_Send_CheckBlock(void)
{
	Uart_1.uart_want_get=UART_CBLOCK;
	make_uartSend_Data((uint8_t *)Uart_CheckBlock_Char,1);
}
const char Uart_Reset_Char[1]={UART_RST};
void Uart_Send_Reset(void)
{
	Uart_1.uart_want_get=UART_RST;
	make_uartSend_Data((uint8_t *)Uart_Reset_Char,1);
}

void Uart_RunToSet(uint8_t Num)
{
	char Uart_RunToSet_Char[2]={UART_RUNTOSET,Num};
	Uart_1.uart_want_get=UART_RUNTOSET;
	make_uartSend_Data((uint8_t *)Uart_RunToSet_Char,2);
}


const char Uart_Start_Char[1]={UART_SYNC};
void Uart_Start(void)
{
	make_uartSend_Data((uint8_t *)Uart_Start_Char,1);
}
const char Uart_CallBack_Char[1]={UART_MAIN};
void Uart_CallBack(void)
{
	make_uartSend_Data((uint8_t *)Uart_CallBack_Char,1);
}
const char Uart_CancelError_Char[1]={RLEASE_PRBLEM};
void Uart_CancelError(void)
{
	Uart_1.uart_want_get=RLEASE_PRBLEM;
	make_uartSend_Data((uint8_t *)Uart_CancelError_Char,1);
}
void Uart_deal_message(uart_str *uart)
{
	switch(uart->datbuf[0])
	{
		case  UART_SYNC:
					Para_str.version=uart->datbuf[1];
					Para_str.time_str.set_Remindtime=uart->datbuf[2];
					Para_str.mode_str.Remind_mode=uart->datbuf[3];
					Para_str.mode_str.Block_mode=uart->datbuf[4];
					Para_str.mode_str.WillBlock_mode=uart->datbuf[4]&0x04;
					Para_str.mode_str.Knock_mode=uart->datbuf[5];
					Para_str.mode_str.unit_mode=uart->datbuf[6];
					Para_str.hight_str.top_hight=(uint16_t)uart->datbuf[7]<<8|uart->datbuf[8];
					Para_str.hight_str.down_hight=(uint16_t)uart->datbuf[9]<<8|uart->datbuf[10];
					Para_str.FirstPower=uart->datbuf[11];
					Para_str.hight_str.set_hight[0]=(uint16_t)uart->datbuf[12]<<8|uart->datbuf[13];
					Para_str.hight_str.set_hight[1]=(uint16_t)uart->datbuf[14]<<8|uart->datbuf[15];
					Para_str.hight_str.set_hight[2]=(uint16_t)uart->datbuf[16]<<8|uart->datbuf[17];
					Para_str.hight_str.set_hight[3]=(uint16_t)uart->datbuf[18]<<8|uart->datbuf[19];
		break;
		case  UART_GETHIGHT:   
					Para_str.hight_str.now_hight=(uint16_t)uart->datbuf[1]<<8|uart->datbuf[2];
					break;
		case  UART_SET: 
					if(uart->datbuf[0]!=uart->uart_want_get)break;
					Para_str.Set_Result=true;
					LCD_Control(WM_ARRIVE);
					break;
		case  UART_IBALENCE:  
		case  UART_CBALENCE:   		
		case  UART_CBLOCK:		  
		case  UART_RST: 
					if(uart->datbuf[0]!=uart->uart_want_get)break;
					Para_str.Set_Result=(uart->datbuf[1])?true:false;
					LCD_Control(WM_ARRIVE);
					break;		
		case  UART_MAIN:
					if(uart->datbuf[1])
					{
						Para_str.problem=uart->datbuf[1];
						lcd_switch(1);
						Creat_Frame(Problem_Frame);
					}
					else if(uart->datbuf[2]==1)
					{
						lcd_switch(1);
						Creat_Frame(Ring_Frame);
					}
					else if(uart->datbuf[2]==2)
					{
						Beep_SetNum(2,2,2);
					}
					Uart_CallBack();
					break;
		case  UART_RUNTOSET:
		case  UART_HIGHTSET:
					if(uart->datbuf[0]!=uart->uart_want_get)break;
					LCD_Control(WM_ARRIVE);
					break;
		case  UART_RUN:
					Para_str.hight_str.now_hight=(uint16_t)uart->datbuf[1]<<8|uart->datbuf[2];
					break;
		case RLEASE_PRBLEM:
					if(uart->datbuf[0]!=uart->uart_want_get)break;
					Para_str.Relaseend=uart->datbuf[1];
					LCD_Control(WM_ARRIVE);
					break;
		case HAVEEND:
					LCD_Control(WM_ARRIVE);
					break;
		default:
			break;
	}
}


void Uart_parse(uart_str *uart)
{
	static uint16_t rx16dat;
	uint8_t dat=0;
	uint8_t ERROR_STATE=0;
	uint16_t Crc_cal=0;
	static char Crc_get[2]={0,0};	
	static uint8_t Uart_Index;
	static uint8_t Uart_lenth;
	static uint8_t Crc_Index;
	if(uart->wait_deal_num==false)return;
	if(uart_read(&dat)==false)
	{
		uart->wait_deal_num=0;
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
			if(dat>3&&dat<24)
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
			rx16dat=rx16dat<<8|dat;
			if(rx16dat==0x5c9d)
			{
				uart->datbuf[uart->rxdat_length-1]=dat;
				if(uart->rx_length==Uart_lenth)
				{
					Crc_Index=0;
					Crc_get[Crc_Index++]=dat;
					Uart_Index=U_CRC;
				}
			}
			if(uart->rx_length==Uart_lenth)
			{
				Crc_Index=0;
				Crc_get[Crc_Index++]=dat;
				Uart_Index=U_CRC;
			}
			else
			{
				uart->datbuf[uart->rxdat_length++]=dat;
			}
			break;
		case U_CRC:
			Crc_get[Crc_Index++]=dat;
			if(Crc_Index==2)
			{
				Crc_cal=CRC16MODBUS(uart->rxbuf+1,uart->rxdat_length+1);
				if(Crc_cal==((uint16_t)Crc_get[0]<<8|Crc_get[1]))
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
				uart->rxdat_length=0;
				Uart_Index=U_IDLE;
				Uart_1.wait_deal_num--;
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
			uart->rxdat_length=0;
			uart->rx_length=0;
			Uart_Index=U_IDLE;
		}
}

