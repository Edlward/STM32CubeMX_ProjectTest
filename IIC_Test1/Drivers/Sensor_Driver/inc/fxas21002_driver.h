/**
  ******************************************************************************
  * File Name          : fxas21002_driver.h
  * Description        : This file provides code for the fxas21002 .
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
#ifndef	_FXAS21002_DRIVER_H
#define	_FXAS21002_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

	 
	 
	 
	 
void	Init_Fxas21002(I2C_HandleTypeDef *hi2c);
void	Read_Fxas21002_Gyro_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_fxas21002_gyro_data_p);	 
	 
void	Init_Fxas21002_Soft(void);
void	Read_Fxas21002_Gyro_Data_Soft(float *	p_f_fxas21002_gyro_data_p);	 
		 
#ifdef __cplusplus
}
#endif	 
	 
#endif	 



	 