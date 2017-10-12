/**
  ******************************************************************************
  * File Name          : icm20602_driver.h
  * Description        : This file provides code for the icm20602 .
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 aibird
  *
  ******************************************************************************
	*
	* Author:
	*
	******************************************************************************
	*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	_ICM20602_DRIVER_H
#define	_ICM20602_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


void	Init_Icm20602(I2C_HandleTypeDef *hi2c);
void	Read_Icm20602_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_icm20602_gyro_data_p,	float *p_f_icm20602_acc_data_p);

void	Init_Icm20602_Soft(void);
void	Read_Icm20602_Data_Soft(float *	p_f_icm20602_gyro_data_p,	float *p_f_icm20602_acc_data_p);	 




#ifdef __cplusplus
}
#endif	 
	 
#endif	 
