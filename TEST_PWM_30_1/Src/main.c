/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

#include "led.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
/* 类型定义 ------------------------------------------------------------------*/
/************************** 输入捕获外部变量 ********************************/
typedef struct              //测量高电平脉宽
{   
	uint8_t   ucFinishFlag;
	uint8_t   ucStartFlag;
	uint16_t  usCtr;
	uint16_t  usPeriod;
}STRUCT_CAPTURE;

STRUCT_CAPTURE strCapture = { 0, 0, 0 };

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//uint16_t cnt0 = 0;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

//	uint32_t ulTmrClk,ulTime;
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);   //开启定时器1的通道1作为PWM的输出
	
//	ulTmrClk = HAL_RCC_GetHCLKFreq()/71;
	HAL_TIM_Base_Start_IT(&htim3);  //启动定时器3
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);  //启动定时器通道输入捕获并开启中断

	/* open uart receive interrupt */
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);

	printf("Hello \n\r");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

		 /* 完成测量高电平脉宽 */
//    if(strCapture.ucFinishFlag == 1 )
//		{
//      /* 计算高电平计数值 */
////			ulTime = strCapture .usPeriod * (0xFFFF) + strCapture .usCtr;
//			ulTime = strCapture .usCtr;
//			/* 打印高电平脉宽时间 */
//			
//			printf("HighTime is %d %d\n",ulTime/1000,ulTime%1000);
////			printf ( ">>测得高电平脉宽时间：%d.%d s\n", ulTime / ulTmrClk, ulTime % ulTmrClk ); 
//			strCapture .ucFinishFlag = 0;			
//		}
		
//		LED1 = !LED1;
//		HAL_Delay(100);
		
//		LED1 = 0;
//		HAL_Delay(100);
//		LED1 = 1;
//		HAL_Delay(100);
		
  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/**
  * 函数功能: 非阻塞模式下定时器的回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	strCapture .usPeriod ++;
}

/**
  * 函数功能: 定时器输入捕获中断回调函数
  * 输入参数: htim：定时器句柄
  * 返 回 值: 无
  * 说    明: 无 
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//	printf("222 \n\r");
	
//	uint16_t cnt1=0,cnt2=0,cnt3=0;
//	uint16_t cntTime = 0;
	uint32_t ulTime;
//	uint32_t ulTmrClk;
//	uint32_t ulVal;
	
	TIM_IC_InitTypeDef sConfigIC;
	
	if ( strCapture .ucStartFlag == 0 )
  {       
    htim3.Instance->CNT=0; // 清零定时器计数
    strCapture .usPeriod = 0;			
    strCapture .usCtr = 0;
    
//    // 配置输入捕获参数，主要是修改触发电平
//    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
//    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
//    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
//    sConfigIC.ICFilter = 0;
//    HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1);
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);    
    strCapture .ucStartFlag = 1;			
  }		
  
  else
  {   
    // 获取定时器计数值
    strCapture .usCtr = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);
    // 配置输入捕获参数，主要是修改触发电平
//    sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
//    sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
//    sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
//    sConfigIC.ICFilter = 0;
//    HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1);
    
		/* 获取定时器时钟周期 */	
//	ulTmrClk = HAL_RCC_GetHCLKFreq()/71;
		
//		ulTime = strCapture .usCtr;
		/* 计算高电平计数值 */
			ulTime = strCapture .usPeriod * (0xFFFF) + strCapture .usCtr;
			/* 打印高电平脉宽时间 */
			
//		ulVal = ulTime /ulTmrClk;
		
			printf("HighTime is %d %d\n",ulTime/1000,ulTime%1000);
		
    // 清除中断标志位
    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
    // 启动输入捕获并开启中断
    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1); 
    strCapture .ucStartFlag = 0;			
    strCapture .ucFinishFlag = 1;    
  }
	
//	if (cntTime == 0)
//	{
//		cnt1 = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);
//	}
//	if ( cntTime == 1 )
//	{
//		cnt2 = HAL_TIM_ReadCapturedValue(&htim3,TIM_CHANNEL_1);
//		if( cnt2 <= cnt1 )
//		{
//			cnt3 = 65536 - cnt1 + cnt2;
//		}
//		cnt3 = cnt2-cnt1;
//		printf("cnt3 is %d %d\n\r",cnt3/1000,cnt3%1000);
//	}
//	
//	printf("cnt3 is %d\n\r",cnt3);
	
	// 清除中断标志位
//    __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1); 
//    // 启动输入捕获并开启中断
//    HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);    			
   	
//		cntTime++;
//		if ( cntTime >= 2 )
//		{
//			cntTime = 0;
//		}
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
