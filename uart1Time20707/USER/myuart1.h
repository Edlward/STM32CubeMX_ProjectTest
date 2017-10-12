#ifndef __MYUART1_H__
#define __MYUART1_H__

#include "stdint.h"
#include "stm32_bsp_uart.h"

extern uint8_t MyTxBuf[30];
void Task_Uart1_Anasyse_Protocol( uint8_t* myRxbuf );


#endif



