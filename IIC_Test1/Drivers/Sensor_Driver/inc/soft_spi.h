#ifndef __SPI_H
#define __SPI_H
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_spi.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//SPI���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#define uchar unsigned char
#define uint  unsigned int
#define ulong unsigned long
	
#define SPI_MISO 				GPIO_PIN_6
#define SPI_MOSI 				GPIO_PIN_7
#define SPI_SCK         GPIO_PIN_5

#define SPI_NSS					GPIO_PIN_4
#define SPI_PORT   			GPIOA

//SPI_HandleTypeDef hspi1;

#define SPI_MISO_0 				HAL_GPIO_WritePin(SPI_PORT, SPI_MISO,GPIO_PIN_RESET)
#define SPI_MISO_1 				HAL_GPIO_WritePin(SPI_PORT, SPI_MISO,GPIO_PIN_SET)
#define SPI_MOSI_0 				HAL_GPIO_WritePin(SPI_PORT, SPI_MOSI,GPIO_PIN_RESET)
#define SPI_MOSI_1 				HAL_GPIO_WritePin(SPI_PORT, SPI_MOSI,GPIO_PIN_SET)

#define SPI_SCK_0 				HAL_GPIO_WritePin(SPI_PORT, SPI_SCK,GPIO_PIN_RESET)
#define SPI_SCK_1 				HAL_GPIO_WritePin(SPI_PORT, SPI_SCK,GPIO_PIN_SET)

#define SPI_NSS_0   			HAL_GPIO_WritePin(SPI_PORT, SPI_NSS,GPIO_PIN_RESET)
#define SPI_NSS_1   			HAL_GPIO_WritePin(SPI_PORT, SPI_NSS,GPIO_PIN_SET)

//#define SPI_90316_CS_LOW()     HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET)
//#define SPI_90316_CS_HIGH()    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET)

//#define SPI1_CS_Pin  				GPIO_PIN_12
//#define SPI1_CS_GPIO_Port   GPIOB

extern SPI_HandleTypeDef hspi1;

extern uint16_t receive_90316_data1;

extern uint16_t receive90316_data1;
extern uint16_t receive90316_data2;
extern uchar senddat[10];
extern float AngleValue;

void delay_ms(uint32_t data);
void delay(uchar time);
void delay_001s(void);
void synch_start(void);
void SPI_Configuration(void);
 				  	    													  
void  SPI2_Init(void);			 						//��ʼ��SPI��
void  SPI2_SetSpeed(uint8_t SpeedSet); 	//����SPI�ٶ�  
void  SPI_TransmitReadByte(void);

unsigned char SPI1_SendByte(uint8_t data);

//uint16_t SPI1_ReadWriteByte(uint8_t data,uint8_t basic_reg);
//uint8_t SPI2_ReadWriteByte(uint8_t TxData);					//SPI���߶�дһ���ֽ�
		 
#endif

