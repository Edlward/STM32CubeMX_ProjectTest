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
//	/* 保证头桢字节正确，并且字段完整 */
	if ( ( ucHeadHightByte == 0x55 )&& ( ucHeadLowByte == 0xAA )  )  
	{
//		HAL_UART_Transmit(&huart1,&ucHeadLowByte,1,60);
//		if ( myRxbuf[2] == 0xFE )  /* 判断地址是否正确 */
//		{
//			ucCmdlength = myRxbuf[3];
//			
//			/* 分别保存飞机型号，异常信息，客户信息 */
//			airCraftmodel = myRxbuf[4];
//			abnormalInf = myRxbuf[5];
//			customerInf = myRxbuf[6];
//			
//			/* 保存 时 分 秒 */
//			hourTime = myRxbuf[7];
//			minuteTime = myRxbuf[8];
//			secondTime = myRxbuf[9];
//			
//			/* 保存GPS信息 */
//			for( i=0; i<3; i++ )
//			{
//				gpsLatitude[uartCnt++] = myRxbuf[10+i];  /* 经度信息 */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsLongitude[uartCnt++] = myRxbuf[14+i];  /* 纬度信息 */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsHeight[uartCnt++] = myRxbuf[10+i];  /* 高度信息 */
//			}
//			uartCnt = 0;
//			for( i=0; i<3; i++ )
//			{
//				gpsTime[uartCnt++] = myRxbuf[14+i];  /* 卫星受时信息 */
//			}
//			uartCnt = 0;
//			
//			/* 计算异或校验和 */
//			for( i=0; i<ucCmdlength+3; i++ )
//			{
//				ucData = myRxbuf[2+i];
//				ucCheckSum ^= ucData;
//			}
//			
//			/* 判断校验，失败就退出 */
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
//	else  /* 头帧字节不正确或者字节不完整就退出 */
//	{
//		return;
//	}

//	HAL_UART_Transmit(&huart1,uc_receivedata,28,60);

	


}


