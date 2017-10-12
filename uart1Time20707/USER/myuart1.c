#include "myuart1.h"
#include "stdint.h"
#include "stm32_bsp_timer.h"
#include "usart.h"



void Task_Uart1_Anasyse_Protocol( uint8_t* myRxbuf )
{
	uint8_t ucHeadHightByte,ucHeadLowByte,ucTailByte,ucModuleAdd,ucCmdlength,ucData,ucCheckSum;
//	uint8_t airCraftmodel,abnormalInf,customerInf;
//	uint8_t hourTime,minuteTime,secondTime;
//	uint8_t gpsLatitude[4],gpsLongitude[4],gpsHeight[4],gpsTime[4];
	
	
	uint8_t i;
	uint8_t tem[16] = { 0x55,0xAA,0x01,0x0A,
											0x01,0xFF,0xFF,0x01,
											0xFF,0xFF,0x00,0x01,
											0x01,0x01,0xF0,0xF0 }; 
//	uint8_t uartCnt = 0;
//	uint8_t uc_receivedata[28];
//	
	ucHeadHightByte = myRxbuf[0];
	ucHeadLowByte = myRxbuf[1];
//	ucTailByte =  myRxbuf[27];
//	
//	/* ��֤ͷ���ֽ���ȷ�������ֶ����� */
	if ( ( ucHeadHightByte == 0x55 )&& ( ucHeadLowByte == 0xAA )  )  
	{
//		HAL_UART_Transmit(&huart1,&ucHeadLowByte,1,60);
//		if ( myRxbuf[2] == 0xFE )  /* �жϵ�ַ�Ƿ���ȷ */
//		{
//			ucCmdlength = myRxbuf[3];
//			
//			/* �ֱ𱣴�ɻ��ͺţ��쳣��Ϣ���ͻ���Ϣ */
//			airCraftmodel = myRxbuf[4];
//			abnormalInf = myRxbuf[5];
//			customerInf = myRxbuf[6];
//			
//			/* ���� ʱ �� �� */
//			hourTime = myRxbuf[7];
//			minuteTime = myRxbuf[8];
//			secondTime = myRxbuf[9];
//			
//			/* ����GPS��Ϣ */
//			for( i=0; i<3; i++ )
//			{
//				gpsLatitude[uartCnt++] = myRxbuf[10+i];  /* ������Ϣ */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsLongitude[uartCnt++] = myRxbuf[14+i];  /* γ����Ϣ */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsHeight[uartCnt++] = myRxbuf[10+i];  /* �߶���Ϣ */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsTime[uartCnt++] = myRxbuf[14+i];  /* ������ʱ��Ϣ */
//			}
//			uartCnt = 0;
//			
//			/* �������У��� */
//			for( i=0; i<ucCmdlength+3; i++ )
//			{
//				ucData = myRxbuf[2+i];
//				ucCheckSum ^= ucData;
//			}
//			
//			/* �ж�У�飬ʧ�ܾ��˳� */
//			if( (0x00 == ucCheckSum) && ( 0xF0 == ucTailByte ) )
//			{
				for ( i=0; i<28; i++ )
				{
					MyTxBuf[i] = tem[i];
				}
//				
//			}
//			
//		}
	}
//	else  /* ͷ֡�ֽڲ���ȷ�����ֽڲ��������˳� */
//	{
//		return;
//	}

//	HAL_UART_Transmit(&huart1,uc_receivedata,28,60);

	


}


