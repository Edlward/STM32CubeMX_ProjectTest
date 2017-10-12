/**
  ******************************************************************************
  * File Name          : mma8452_driver.h
  * Description        : This file provides code for the mma8452 .
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
#ifndef	_MMA8452_DRIVER_H
#define	_MMA8452_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

	 	 
	 
void	Init_Mma8452(I2C_HandleTypeDef *hi2c);
void	Read_Mma8452_Acc_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_mma8452_acc_data_p);	 

	 
void	Init_Mma8452_Soft(void);
void	Read_Mma8452_Acc_Data_Soft(float *	p_f_mma8452_acc_data_p);	 
	 
#ifdef __cplusplus
}
#endif

#endif	 















