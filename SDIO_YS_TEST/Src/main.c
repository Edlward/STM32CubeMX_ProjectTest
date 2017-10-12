/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "fatfs.h"
#include "sdio.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "ff_gen_drv.h"
#include "sd_diskio.h"


/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

char SDPath[4];  //SD卡逻辑设备路径
FRESULT f_res = 0;  //文件操作结果
FATFS fs;   //FatFs文件系统对象
FIL file;   //文件对象
UINT funm;  //文件成功读写数量
BYTE ReadBuffer[1024]={0};  //读缓冲区
BYTE WriteBuffer[]="\r\n从明天起，做一个幸福的人 \r\n喂马，劈柴，周游世界\n";  //写缓冲区

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
static void printf_fatfs_error(FRESULT fresult);
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

	printf("**********这是一个基于串行FLASH的FATFS文件系统实验***********\r\n");
	
	///* 注册一个FatFs设备：SD卡 */
	if ( FATFS_LinkDriver(&SD_Driver,SDPath) == 0 )
	{
		//在SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
		f_res = f_mount( &fs,(TCHAR const*)SDPath,1 );
		printf_fatfs_error(f_res);
		
		///*-------------格式化测试---------------------*/
		///* 如果没有文件系统就格式化创建文件系统 */
		if( f_res == FR_NO_FILESYSTEM )
		{
			printf(" >> SD卡还没有文件系统，即将进行格式化... \n ");
			///* 格式化 */
			f_res = f_mkdir( (TCHAR const*)SDPath );
			
			if ( f_res == FR_OK )
			{
				printf(" >> SD卡已经成功格式化文件系统... \n ");
				///* 格式化后，先取消挂载 */
				f_res = f_mount(NULL,(TCHAR const*)SDPath,1);
				///* 重新挂载 */
				f_res = f_mount(&fs,(TCHAR const*)SDPath,1);
				
			}
			else
			{
				printf("<<格式化失败...>> \n");
			}
			
		}
		else if( f_res != FR_OK )
		{
			printf("！！SD卡挂载文件系统失败...\n");
			printf_fatfs_error(f_res);
		}
		else
		{
			printf(">> 文件系统挂载成功，可以进行读写测试 \n");
		}
		
		///*************文件系统测试--写测试***********************/
		///*打开文件，如果不存在就创建*/
		f_res=f_open(&file,"FatFs读写测试文件.txt",FA_CREATE_ALWAYS|FA_WRITE);
		if( f_res == FR_OK )
		{
			printf(">> 打开、创建FATFS读写测试文件成功，想文件写入数据。\n");
			///*将制定存储区的内容写入到文件内*/
			f_res = f_write(&file,WriteBuffer,sizeof(WriteBuffer),&funm);
			if(f_res==FR_OK)
			{
				printf(">>文件写入成功，写入字节数数据：%d\n",funm);
				printf(">>向文件写入的数据为：\n%s\n",WriteBuffer);
			}
			else
			{
				printf("!!文件写入失败：%d\n",f_res);
			}
			///*关闭文件*/
			f_close(&file);
		}
		else
		{
			printf("!!打开创建文件失败。\n");
		}
		
		///*-------------------文件系统测试：读测试--------------------------*/
		f_res = f_open(&file, "FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
    if(f_res == FR_OK)
    {
      printf("》打开文件成功。\n");
      f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &funm); 
      if(f_res==FR_OK)
      {
        printf("》文件读取成功,读到字节数据：%d\n",funm);
        printf("》读取得的文件数据为：\n%s \n", ReadBuffer);	
      }
      else
      {
        printf("！！文件读取失败：(%d)\n",f_res);
      }		
    }
    else
    {
      printf("！！打开文件失败。\n");
    }
    /* 不再读写，关闭文件 */
    f_close(&file);
    
    /* 不再使用，取消挂载 */
    f_res = f_mount(NULL,(TCHAR const*)SDPath,1);	
  }
    
  /* 注销一个FatFS设备：SD卡 */
  FATFS_UnLinkDriver(SDPath);

	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

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
  * 函数功能: FatFS文件系统操作结果信息处理.
  * 输入参数: FatFS文件系统操作结果：FRESULT
  * 返 回 值: 无
  * 说    明: 无
  */
static void printf_fatfs_error(FRESULT fresult)
{
  switch(fresult)
  {
    case FR_OK:                   //(0)
      printf("》操作成功。\n");
    break;
    case FR_DISK_ERR:             //(1)
      printf("！！硬件输入输出驱动出错。\n");
    break;
    case FR_INT_ERR:              //(2)
      printf("！！断言错误。\n");
    break;
    case FR_NOT_READY:            //(3)
      printf("！！物理设备无法工作。\n");
    break;
    case FR_NO_FILE:              //(4)
      printf("！！无法找到文件。\n");
    break;
    case FR_NO_PATH:              //(5)
      printf("！！无法找到路径。\n");
    break;
    case FR_INVALID_NAME:         //(6)
      printf("！！无效的路径名。\n");
    break;
    case FR_DENIED:               //(7)
    case FR_EXIST:                //(8)
      printf("！！拒绝访问。\n");
    break;
    case FR_INVALID_OBJECT:       //(9)
      printf("！！无效的文件或路径。\n");
    break;
    case FR_WRITE_PROTECTED:      //(10)
      printf("！！逻辑设备写保护。\n");
    break;
    case FR_INVALID_DRIVE:        //(11)
      printf("！！无效的逻辑设备。\n");
    break;
    case FR_NOT_ENABLED:          //(12)
      printf("！！无效的工作区。\n");
    break;
    case FR_NO_FILESYSTEM:        //(13)
      printf("！！无效的文件系统。\n");
    break;
    case FR_MKFS_ABORTED:         //(14)
      printf("！！因函数参数问题导致f_mkfs函数操作失败。\n");
    break;
    case FR_TIMEOUT:              //(15)
      printf("！！操作超时。\n");
    break;
    case FR_LOCKED:               //(16)
      printf("！！文件被保护。\n");
    break;
    case FR_NOT_ENOUGH_CORE:      //(17)
      printf("！！长文件名支持获取堆空间失败。\n");
    break;
    case FR_TOO_MANY_OPEN_FILES:  //(18)
      printf("！！打开太多文件。\n");
    break;
    case FR_INVALID_PARAMETER:    // (19)
      printf("！！参数无效。\n");
    break;
  }
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
