/**
  ******************************************************************************
  * File Name          : as5600_driver.c
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
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "as5600_driver.h"

#include "soft_iic1.h"

/* Private define ------------------------------------------------------------*/
/** @defgroup AS5600 Device And Register Address
  
  */
#define		AS5600_DEVICE_ADDRESS													(0x6C)		/* */
#define		AS5600_CONF_MSB_REG_ADDRESS										(0x07)
#define		AS5600_CONF_LSB_REG_ADDRESS										(0x08)
#define		AS5600_RAW_ANGLE_MSB_REG_ADDRESS							(0x0C)		//ԭʼ�Ƕ�����MSB
#define		AS5600_RAW_ANGLE_LSB_REG_ADDRESS							(0x0D)		//ԭʼ�Ƕ�����LSB
#define		AS5600_ANGLE_REG_MSB_ADDRESS									(0x0E)    //�����Ƕ�����MSB
#define		AS5600_ANGLE_REG_LSB_ADDRESS									(0x0F)		//�����Ƕ�����LSB
#define		AS5600_STATUS_REG_ADDRESS											(0x0B)		//״̬�Ĵ���
#define		AS5600_AGC_REG_ADDRESS												(0x1A)		//�ų��Զ�����Ĵ���
#define		AS5600_MAGNITUDE_MSB_REG_ADDRESS							(0x1B)		//�ں˱���ų�ǿ�ȼĴ���MSB
#define		AS5600_MAGNITUDE_LSB_REG_ADDRESS							(0x1C)		//�ں˱���ų�ǿ�ȼĴ���LSB




/* Private variables ---------------------------------------------------------*/
/**
  * @brief 	init AS5600 				
  */
void	Init_As5600(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_as5600_reg_config_value_p;
	/**
		* AS5600_CONF_LSB_REG_ADDRESS
		* bit1:0	power_mode:00-NORMAL 01-LPM1 10-LPM2 11-LPM3
		* bit3:2	Hysteresis:00-OFF    01-1SLB 10-2LSB 11-3LSB
		* bit5:4	Output:    00-analog(0-100%) 01-analog(10-20%) 10-PWM
		* bit7:6	PWM Freq:	 00-115    01-230  10-460  11-920
		*/
	uc_as5600_reg_config_value_p = 0x20;   //standby
	HAL_I2C_Mem_Write(hi2c,	AS5600_DEVICE_ADDRESS,	AS5600_CONF_LSB_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_as5600_reg_config_value_p,	1,	100);
	/**
		* AS5600_CONF_MSB_REG_ADDRESS
		* bit9:8	slow flt:  00-16x    01-8x   10-4x   11-2x
		* bit12:10fast flt thr:
							000-slow flt only   001-6LSB  010-7LSB  011-9LSB
							100-18LSB						101-21LSB	110-24LSB 111-10LSB	
	
		* bit13		watchdog:   0-OFF		1-ON
		*/
}


/**
  * @brief get as5600 angle
	* @note	 	��Ҫ������λ�÷���as5600
	*					�Ƕ�����Ϊ16λ�޷������Σ���Чλ��Ϊbit0-bit11
	* @param hi2c : Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
	* @param uw_as5600_angle_p��uint16_t���͵�ָ��
  */

void	Read_As5600_Angle_Data(I2C_HandleTypeDef *hi2c,	uint16_t *	uw_as5600_angle_p)
{
	static	FlagStatus	st_first_read_as5600_angle_reg_data_flag_p = SET;
	uint8_t	uc_as56000_angle_reg_data_p[2];
	
	
	if(SET == st_first_read_as5600_angle_reg_data_flag_p)
	{
		/*��ȡһ�νǶȼĴ�����ֵ��ʹauto_increment pointer������angle_reg,��֤������ȡ����Ҫ�����ڲ��Ĵ�����ַ*/
		HAL_I2C_Mem_Read(hi2c,	AS5600_DEVICE_ADDRESS,	AS5600_ANGLE_REG_MSB_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_as56000_angle_reg_data_p,	2,	100);	
		*uw_as5600_angle_p = ( ((uint16_t)uc_as56000_angle_reg_data_p[0])<<8 )|uc_as56000_angle_reg_data_p[1];
		
		st_first_read_as5600_angle_reg_data_flag_p = RESET;
		return;
	}
	else
	{
		HAL_I2C_Master_Receive(hi2c,	AS5600_DEVICE_ADDRESS,	uc_as56000_angle_reg_data_p,	2,	10);
		*uw_as5600_angle_p = ( ((uint16_t)uc_as56000_angle_reg_data_p[0])<<8 )|uc_as56000_angle_reg_data_p[1];
	}
}




/*---------------------------------ģ��I2C��ȡAS5600�Ƕ�------------------------*/
/**
  * @brief 	init AS5600 				
  */
void	Init_As5600_Soft(void)
{
	uint8_t	uc_as5600_reg_config_value_p;
	/**
		* AS5600_CONF_LSB_REG_ADDRESS
		* bit1:0	power_mode:00-NORMAL 01-LPM1 10-LPM2 11-LPM3
		* bit3:2	Hysteresis:00-OFF    01-1SLB 10-2LSB 11-3LSB
		* bit5:4	Output:    00-analog(0-100%) 01-analog(10-20%) 10-PWM
		* bit7:6	PWM Freq:	 00-115    01-230  10-460  11-920
		*/
	uc_as5600_reg_config_value_p = 0x20;   									//standby
	I2C_Write_Single_Reg_1(AS5600_DEVICE_ADDRESS,AS5600_CONF_LSB_REG_ADDRESS,uc_as5600_reg_config_value_p);
	/**
		* AS5600_CONF_MSB_REG_ADDRESS
		* bit9:8	slow flt:  00-16x    01-8x   10-4x   11-2x
		* bit12:10fast flt thr:
							000-slow flt only   001-6LSB  010-7LSB  011-9LSB
							100-18LSB						101-21LSB	110-24LSB 111-10LSB	
	
		* bit13		watchdog:   0-OFF		1-ON
		*/
}
/**
  * @brief get as5600 angle
	* @note	 	��Ҫ������λ�÷���as5600
	*					�Ƕ�����Ϊ16λ�޷������Σ���Чλ��Ϊbit0-bit11
	* @param hi2c : Pointer to a I2C_HandleTypeDef structure that contains
  *                the configuration information for the specified I2C.
	* @param uw_as5600_angle_p��uint16_t���͵�ָ��
  */

void	Read_As5600_Angle_Data_Soft(uint16_t *	uw_as5600_angle_p)
{
	static	FlagStatus	st_first_read_as5600_angle_reg_data_flag_p = SET;
	uint8_t	uc_as56000_angle_reg_data_p[2];
		
	if(SET == st_first_read_as5600_angle_reg_data_flag_p)
	{
		/*��ȡһ�νǶȼĴ�����ֵ��ʹauto_increment pointer������angle_reg,��֤������ȡ����Ҫ�����ڲ��Ĵ�����ַ*/
		I2C_Read_Multi_Reg_1(AS5600_DEVICE_ADDRESS,AS5600_ANGLE_REG_MSB_ADDRESS,uc_as56000_angle_reg_data_p,2);
		*uw_as5600_angle_p = ( ((uint16_t)uc_as56000_angle_reg_data_p[0])<<8 )|uc_as56000_angle_reg_data_p[1];
		
		st_first_read_as5600_angle_reg_data_flag_p = RESET;
		return;
	}
	else
	{
		I2C_Read_Multi_Reg_1(AS5600_DEVICE_ADDRESS,AS5600_ANGLE_REG_MSB_ADDRESS,uc_as56000_angle_reg_data_p,2);
		*uw_as5600_angle_p = ( ((uint16_t)uc_as56000_angle_reg_data_p[0])<<8 )|uc_as56000_angle_reg_data_p[1];
	}
}







