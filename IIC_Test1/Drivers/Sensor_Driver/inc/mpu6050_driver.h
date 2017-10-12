/**
  ******************************************************************************
  * File Name          : mpu6050_driver.h
  * Description        : This file provides code for the mpu605 .
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
#ifndef _MPU6050_DRIVER_H
#define _MPU6050_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


void	Init_Mpu6050(I2C_HandleTypeDef *hi2c);
void	Read_Mpu6050_Data(I2C_HandleTypeDef *hi2c,float *	p_f_mpu6050_gyro_data_p,	float	*p_f_mpu6050_acc_data_p);

void	Init_Mpu6050_Soft(void);
void	Read_Mpu6050_Data_Soft(float *	p_f_mpu6050_gyro_data_p,	float	*p_f_mpu6050_acc_data_p);	 
	 

#ifdef __cplusplus
}
#endif	 

#endif
