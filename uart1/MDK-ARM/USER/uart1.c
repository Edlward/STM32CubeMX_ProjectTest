#include "uart1.h"



/*******************************************************************************
* ������  :  	  uart_GetChar
* ��������:       �ӽ��ջ�������һ�ֽڣ���������ֽڣ���ȷ���������������ݡ�
* �������:       - UART_BUF * ptRx: ���ջ������ṹ����׵�ַ
*
* �������:       ��
* �� �� ֵ:       �ӽ��ջ�������ȡ�Ǹ��ֽڵ�����
* ����˵���� 
* �޸�����    	  �汾��     �޸���	     �޸�����
* -----------------------------------------------
* 2010/06/11      V1.0	     00574zsm	       XXXX
*******************************************************************************/

uint8_t UART_GetChar(USART_Buf_TypeDef * ptRx)
{	
	uint8_t ucData;
	ucData = *((*ptRx).pcBufAddr + (*ptRx).ucBufRdInde);
	if ((*ptRx).ucBufCnt != 0)
	{	
		if (++(*ptRx).ucBufRdInde == (*ptRx).ucBufSize)
		{
		   (*ptRx).ucBufRdInde = 0;
		}
		__HAL_UART_DISABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�ؽ����ж�

		--(*ptRx).ucBufCnt;
		__HAL_UART_ENABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//�������ж�
	}
	
	return ucData;
}


