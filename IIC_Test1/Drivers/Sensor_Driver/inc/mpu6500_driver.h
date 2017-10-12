/**
  ******************************************************************************
  * File Name          : mpu6500_driver.h
  * Description        : This file provides code for the mpu6500.
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
#ifndef _MPU6500_DRIVER_H
#define _MPU6500_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


void  Init_Mpu6500(I2C_HandleTypeDef *hi2c);
void  Read_Mpu6500_Data(I2C_HandleTypeDef *hi2c,float * p_f_mpu6500_gyro_data_p,  float *p_f_mpu6500_acc_data_p);

void  Init_Mpu6500_Soft(void);
void  Read_Mpu6500_Data_Soft(float *  p_f_mpu6500_gyro_data_p,  float *p_f_mpu6500_acc_data_p);  
   

#ifdef __cplusplus
}
#endif   

#endif
