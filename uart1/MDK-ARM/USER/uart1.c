#include "uart1.h"



/*******************************************************************************
* 函数名  :  	  uart_GetChar
* 函数描述:       从接收缓冲区读一字节，并清除此字节，需确保缓冲区内有数据。
* 输入参数:       - UART_BUF * ptRx: 接收缓冲区结构体的首地址
*
* 输出参数:       无
* 返 回 值:       从接收缓冲区读取那个字节的数据
* 其它说明： 
* 修改日期    	  版本号     修改人	     修改内容
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
		__HAL_UART_DISABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//关接收中断

		--(*ptRx).ucBufCnt;
		__HAL_UART_ENABLE_IT(ptRx->ptUartHandel,UART_IT_RXNE);//开接收中断
	}
	
	return ucData;
}


