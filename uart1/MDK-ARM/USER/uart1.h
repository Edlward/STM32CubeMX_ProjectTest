#ifndef __UART1_H__
#define __UART1_H__

#include "stdint.h"

typedef struct								    //��������������������غ����Ĳ���
{	
	//	User_Timer_Typedef *st_timer;
		uint8_t  	ucBufSize;							//���������飺��������С
		volatile	uint8_t  ucBufWrInde;						//���������飺дָ��
		volatile	uint8_t  ucBufRdInde;						//���������飺��ָ��
		volatile	uint8_t  ucBufCnt;							//���������飺���ݼ���
		volatile	uint8_t  ucBufOvf;							//���������飺�����־��ע������
		uint8_t  * pcBufAddr;						//���������飺�׵�ַָ�����
	//	UART_HandleTypeDef	*ptUartHandel;
} USART_Buf_TypeDef;

#endif



