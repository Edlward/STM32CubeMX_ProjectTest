#include "stm32f1xx_hal.h"
#include "soft_spi.h"
//#include "spi_5012_cmd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI2的初始化
uint8_t MISO_frame[10];
uint8_t MOSI_frame[10];

uint8_t val_90316_buff_data[2];
//uint8_t send_buff_aval[2]; //获取5012角度值
//uint8_t send_buff_aspd[2]; //获取5012角速度值
//uint8_t send_buff_arev[2]; //获取5012角度转数，在完整逆时针旋转时，递减，在每个完整顺时针旋转时，递增
uint16_t receive90316_data1;
uint16_t receive90316_data2;
float AngleValue;
//signed  short int  receive_Aspd;

uchar senddat[10];

void DIEA(uint8_t *RET_frame);
/****************延时函数***************************/
void delay_ms(uint32_t data)
{
	HAL_Delay(data);
}
/*****************代码延时函数***********************/
void delay(uchar time)
{
		for(;time>0;time--);
}
/**********************延时函数**********************/
void delay_001s(void)
{
 uchar i,j,k;
 for(i=13;i>0;i--)
  {
		for(j=5;j>0;j--)
    {
			for(k=142;k>0;k--);
		}
  }
}
/********************同步信号***********************/
void synch_start(void)
{
  SPI_NSS_1;
	delay_001s();
  SPI_SCK_0;
	SPI_NSS_0;
	delay(50);
}
/*
	SPI1函数初始化
*/
void SPI_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = SPI_MOSI;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化GPIOB
	
  GPIO_InitStructure.Pin = SPI_MISO;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  		//PB13/14/15复用推挽输出 
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = SPI_SCK;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  //PB13/14/15复用推挽输出 
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);		
	
	GPIO_InitStructure.Pin = SPI_NSS;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;  //PB13/14/15复用推挽输出 
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化GPIOB
	
	//SPI_MISO_1;
//	SPI_NSS_1;
//	SPI_SCK_1;
//	SPI_MOSI_1;
//	SPI_NSS_1;
	
	//SPI_5012_IFA_LOW();
	//SPI_5012_IFB_LOW();
}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节

/*************************单字节数据发送**************************/
unsigned char SPI1_SendByte(uint8_t data)
{
	 uint8_t icoff,jcoff;
	 uint8_t Bitflag;
	 jcoff = 0;
	 SPI_SCK_0;
	
	 for(icoff=0;icoff<8;icoff++)
	 {
			delay(20);
		  SPI_SCK_1;
		  SPI_MOSI_1;
		  delay(20);
		  //delay_ms(2);
		 
		  jcoff <<= 1;
		 
		  Bitflag = HAL_GPIO_ReadPin(GPIOA,SPI_MISO);
		 
		  if(Bitflag == 1)
			{
				jcoff |= 0x01;
			}	
			else
			{
				jcoff &= 0xFE;
			}
			
			if((data&0x80)!=0)
			{
				SPI_MOSI_1;
			}
			else
			{
				SPI_MOSI_0;
			}
			data<<=1;
			delay(10);
			SPI_SCK_0;
			
			delay(20);
		}	 
	  SPI_SCK_0;
	  return jcoff;
			
}
/***********************收发数据*************************/
void  SPI_TransmitReadByte(void)
{	
	 uchar iclass;

   SPI_NSS_1;
	 SPI_SCK_0;

	 synch_start();
	 delay(20);
	 senddat[0]=SPI1_SendByte(0xAA);
	 delay(30);//delay(30);
	 senddat[1]=SPI1_SendByte(0xFF);
	 delay(100);
	 for(iclass=0;iclass<8;iclass++)
	 {
		 senddat[iclass+2]=SPI1_SendByte(0xFF);
		 delay(100);		
	 }
	 delay(100);
 	 SPI_SCK_0;
	 SPI_NSS_1;

	 receive90316_data1 = ((uint16_t)(senddat[2]<<8)|(senddat[3]));
	 receive90316_data1 >>= 2;
	 receive90316_data1 &= 0x3FFF;
	 
	 AngleValue = (float)(((float)receive90316_data1/16384)*360);   //转换360°范围
	 
	 receive90316_data2 = ((uint16_t)(senddat[4]<<8)|(senddat[5]));
}
/***********************************************************/
//void SPI_InMode(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;
//	
//		GPIO_InitStructure.Pin = SPI_MOSI;
//		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;  //PB13/14/15复用推挽输出 
//		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
//		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
//void SPI_OutMode(void)
//{
//		GPIO_InitTypeDef GPIO_InitStructure;
//	
//		GPIO_InitStructure.Pin = SPI_MOSI;
//		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;  //PB13/14/15复用推挽输出 
//		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
//		HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
/***********************************************************/
//void ExchangeSPIframe (uint8_t *MOSI_frame, uint8_t *MISO_frame)
//{
//    int i;
//    SPI_NSS_0;
//    delay(7);
//		for(i = 0; i < 10;i++)
//		{
////			SPI_OutMode();
////			HAL_SPI_Transmit(&hspi1,&MOSI_frame[i],1,500);
//			MISO_frame[i] = SPI1_SendByte( MOSI_frame[i]);
//			delay(45);
////			SPI_InMode();
////			HAL_SPI_Receive(&hspi1,&MISO_frame[i],1,500);
////			delay(20);
//		}
//    SPI_NSS_1;
//	
////	  receive90316_data1 = ((uint16_t)(MISO_frame[2]<<8)|(MISO_frame[3]));
////	  receive90316_data2 = ((uint16_t)(MISO_frame[4]<<8)|(MISO_frame[5]));
//		
//}

//*******************************************************************************************************
// void DIEA(uint8_t *RET_frame)
//{
//    uint8_t DIEA_frame[10];
//   // Send SPI  0xAA FF  FF FF FF FF FF F, 90316 Die A
//	
//	  DIEA_frame[0] = 0xAA;
//    DIEA_frame[1] = 0xFF;
//	
//    DIEA_frame[2] = 0xFF;
//    DIEA_frame[3] = 0xFF;
//    DIEA_frame[4] = 0xFF;
//    DIEA_frame[5] = 0xFF;
//    DIEA_frame[6] = 0xFF;
//    DIEA_frame[7] = 0xFF;
//    DIEA_frame[8] = 0xFF;
//    DIEA_frame[9] = 0xFF;
//	
////    DIEA_frame[0] = 0xAA;
////    DIEA_frame[1] = 0xFF;
////	
////    DIEA_frame[2] = 0xFF;
////    DIEA_frame[3] = 0xFF;
////    DIEA_frame[4] = 0xFF;
////    DIEA_frame[5] = 0xFF;
////    DIEA_frame[6] = 0xFF;
////    DIEA_frame[7] = 0xFF;
////    DIEA_frame[8] = 0xFF;
////    DIEA_frame[9] = 0xFF;
//    
//    ExchangeSPIframe (DIEA_frame, RET_frame);
//    //CheckMISOframeForErrors (RET_frame, OpCodeMISO_NOP); // to be checked
//}
///**************************************************************/
//void  SPI_TransmitReadByte(void)
//{
//   SPI_NSS_1;
//	 SPI_SCK_0;

//	 synch_start();
//   delay(20);  
//	 DIEA(MISO_frame);
//	
//	 delay(100); 
//	 SPI_SCK_0;
//	 SPI_NSS_1;
//}		
/**************************************************************/


























