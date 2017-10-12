/**
  ******************************************************************************
  * File Name          : as5600_driver.h
  * Description        : This file provides code for the as5600 .
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
#ifndef	_AS5600_DRIVER_H
#define	_AS5600_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

void	Init_As5600(I2C_HandleTypeDef *hi2c);
void	Read_As5600_Angle_Data(I2C_HandleTypeDef *hi2c,	uint16_t *	uw_as5600_angle_p);
	

void	Init_As5600_Soft(void);
void	Read_As5600_Angle_Data_Soft(uint16_t *	uw_as5600_angle_p);	 





#ifdef __cplusplus
}
#endif


#endif
