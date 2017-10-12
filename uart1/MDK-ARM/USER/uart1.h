#ifndef __UART1_H__
#define __UART1_H__

#include "stdint.h"

typedef struct								    //缓冲区各变量，用于相关函数的参数
{	
	//	User_Timer_Typedef *st_timer;
		uint8_t  	ucBufSize;							//缓冲区数组：缓冲区大小
		volatile	uint8_t  ucBufWrInde;						//缓冲区数组：写指针
		volatile	uint8_t  ucBufRdInde;						//缓冲区数组：读指针
		volatile	uint8_t  ucBufCnt;							//缓冲区数组：数据计数
		volatile	uint8_t  ucBufOvf;							//缓冲区数组：溢出标志，注意清零
		uint8_t  * pcBufAddr;						//缓冲区数组：首地址指针变量
	//	UART_HandleTypeDef	*ptUartHandel;
} USART_Buf_TypeDef;

#endif



