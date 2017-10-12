#include "myuart1.h"
#include "stm32f1xx_hal.h"
#include "usart.h"



//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//		if ( huart->Instance == USART1 )
//		{
//			HAL_UART_Transmit_IT(&huart1,huart1.pRxBuffPtr-huart1.RxXferSize,huart1.RxXferSize);
//			HAL_UART_Receive_IT(&huart1,uart1_RxBuffer,5);
//			HAL_UART_Transmit_IT(&huart1,uart1_RxBuffer,5);
//			__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
//		}

//}


