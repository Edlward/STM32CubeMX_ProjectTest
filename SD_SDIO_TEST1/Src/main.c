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
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

#define BLOCK_SIZE            512         // SD�����С     
#define NUMBER_OF_BLOCKS      8           // ���Կ�����(С��15)
#define WRITE_READ_ADDRESS    0x00000000  // ���Զ�д��ַ

SD_HandleTypeDef hsdcard;
uint32_t Buffer_Block_Tx[BLOCK_SIZE*NUMBER_OF_BLOCKS]; // д���ݻ���
uint32_t Buffer_Block_Rx[BLOCK_SIZE*NUMBER_OF_BLOCKS]; // �����ݻ���
HAL_SD_ErrorTypedef sd_status;    // HAL�⺯������SD����������ֵ���������
TestStatus test_status;           // ���ݲ��Խ��
	HAL_SD_TransferStateTypedef State;	

void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset);
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength);
TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength);

void SD_EraseTest(void);
void SD_Write_Read_Test(void);

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

//	HAL_SD_TransferStateTypedef State;	
	
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */

	printf("Hello World!\n\r");

	State = HAL_SD_GetStatus(&hsdcard);
	if( State == SD_TRANSFER_OK )
	{
		printf("SD����ʼ���ɹ�");
	}
	else
	{
		printf("SD����ʼ��ʧ��");
		while(1);  //ͣ��
	}
	 /* ��ȡSD����Ϣ */
  sd_status=HAL_SD_Get_CardInfo(&hsdcard,&SDCardInfo);
  if(sd_status==SD_OK)
  {
    printf( "CardType is ��%d\n", SDCardInfo.CardType );
    printf( "CardCapacity is ��0x%llX\n", SDCardInfo.CardCapacity );
    printf( "CardBlockSize is ��%d\n", SDCardInfo.CardBlockSize );
    printf( "RCA is ��%d\n", SDCardInfo.RCA);
    printf( "ManufacturerID is ��%d \n", SDCardInfo.SD_cid.ManufacturerID ); 
  }
  
  /* �������� */
  SD_EraseTest();
  /* ��д���� */
  SD_Write_Read_Test();

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
  * ��������: SD����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SD_EraseTest(void)
{ 
	/* ��1������ΪSD���������2������Ϊ������ʼ��ַ����3������Ϊ����������ַ */
  sd_status=HAL_SD_Erase(&hsdcard,WRITE_READ_ADDRESS,WRITE_READ_ADDRESS+BLOCK_SIZE*NUMBER_OF_BLOCKS*4);
  
  if (sd_status == SD_OK)
  {	
    /* ��ȡ�ող��������� */
    sd_status = HAL_SD_ReadBlocks(&hsdcard,Buffer_Block_Rx,WRITE_READ_ADDRESS,BLOCK_SIZE,NUMBER_OF_BLOCKS);
    
    /* �Ѳ�������������Ա� */
    test_status = eBuffercmp(Buffer_Block_Rx,BLOCK_SIZE*NUMBER_OF_BLOCKS);
    
    if(test_status == PASSED)
      printf("���������Գɹ���\n" ); 
    else	  
      printf("���������ɹ������ݳ���\n" );      
  }
  else
  {
    printf("����������ʧ�ܣ�����SD��֧�ֲ�����ֻҪ��д����ͨ������\n" );
  }
}

/**
  * ��������: SD����д����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SD_Write_Read_Test(void)
{  
  /* ������ݵ�д���� */
  Fill_Buffer(Buffer_Block_Tx,BLOCK_SIZE*NUMBER_OF_BLOCKS, 0x32F1);
  
  /* ��SD��д������ */
  sd_status = HAL_SD_WriteBlocks(&hsdcard,Buffer_Block_Tx,WRITE_READ_ADDRESS,BLOCK_SIZE,NUMBER_OF_BLOCKS);
  printf("write status:%d\n",sd_status);
  HAL_Delay(500);
  
  /* ��SD����ȡ���� */
  sd_status = HAL_SD_ReadBlocks(&hsdcard,Buffer_Block_Rx,WRITE_READ_ADDRESS,BLOCK_SIZE,NUMBER_OF_BLOCKS);
  printf("read status:%d\n",sd_status);
  
  /* �Ƚ����� */
  test_status = Buffercmp(Buffer_Block_Tx, Buffer_Block_Rx, BLOCK_SIZE*NUMBER_OF_BLOCKS/4);	//�Ƚ�
  if(test_status == PASSED)
    printf("����д���Գɹ���\n" );
  else  
  	printf("����д����ʧ�ܣ�\n " );  
}

/**
  * ��������: �ڻ���������д����
  * �������: pBuffer��Ҫ���Ļ�����
  *           BufferLength��Ҫ���Ĵ�С
  *           Offset�����ڻ������ĵ�һ��ֵ 
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void Fill_Buffer(uint32_t *pBuffer, uint32_t BufferLength, uint32_t Offset)
{
  uint32_t index = 0;
  /* ������� */
  for (index = 0; index < BufferLength; index++ )
  {
    pBuffer[index] = index + Offset;
  }
}

/**
  * ��������: �Ƚ������������е������Ƿ����
  * �������: pBuffer1��Ҫ�ȽϵĻ�����1��ָ��
  *           pBuffer2��Ҫ�ȽϵĻ�����2��ָ��
  *           BufferLength������������
  * �� �� ֵ: PASSED�����
  *           FAILED������
  * ˵    ��: ��
  */
TestStatus Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    if(BufferLength%50==0)
    {
      printf("buf:0x%08X - 0x%08X\n",*pBuffer1,*pBuffer2);
    }
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    pBuffer1++;
    pBuffer2++;
  }
  return PASSED;
}

/**
  * ��������: ��黺�����������Ƿ�Ϊ0xff��0
  * �������: pBuffer��Ҫ�ȽϵĻ�������ָ��
  *           BufferLength������������
  * �� �� ֵ: PASSED��������������ȫΪ0xff��0
  *           FAILED��������������������һ����Ϊ0xff��0 
  * ˵    ��: ��
  */
TestStatus eBuffercmp(uint32_t* pBuffer, uint32_t BufferLength)
{
  while (BufferLength--)
  {
    /* SD��������Ŀ���ֵΪ0xff��0 */
    if ((*pBuffer != 0xFFFFFFFF) && (*pBuffer != 0))
    {
      return FAILED;
    }
    pBuffer++;
  }
  return PASSED;
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
