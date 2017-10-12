/**
  ******************************************************************************
  * File Name          : fxas21002_driver.c
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
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "fxas21002_driver.h"

#include "soft_iic.h"



/* Private define ------------------------------------------------------------*/
/** @defgroup FXAS21002 Device And Register Address
  
  */
	
#define		FXAS21002_DEVICE_ADDRESS									(0x42)  //0x40@SA0=0  0x42@SA0=1 

#define		FXAS21002_ID_REG_ADDRESS									(0x0C)  //who am i

#define		FXAS21002_TEMP_REG_ADDRESS								(0x12)	//temperature reg
#define		FXAS21002_CTRL0_REG_ADDRESS								(0x0D)  //BW SPIW SEL[1:0] HPF_EN	FS[1:0]
#define		FXAS21002_CTRL1_REG_ADDRESS								(0x13)  //RST ST DR[2:0] ACTIVE READY 
#define		FXAS21002_CTRL2_REG_ADDRESS								(0x14)  //INT Config
#define		FXAS21002_CTRL3_REG_ADDRESS								(0x15)  //WRAPTOONE(auto increment) EXTCTRLEN FS_DOUBLE

#define		FXAS21002_STATUS_REG_ADDRESS							(0x00)  //copy of DR_STATUS or F_STAUTS
#define		FXAS21002_OUT_X_MSB_REG_ADDRESS						(0x01)
#define		FXAS21002_OUT_X_LSB_REG_ADDRESS						(0x02)
#define		FXAS21002_OUT_Y_MSB_REG_ADDRESS						(0x03)
#define		FXAS21002_OUT_Y_LSB_REG_ADDRESS						(0x04)
#define		FXAS21002_OUT_Z_MSB_REG_ADDRESS						(0x05)
#define		FXAS21002_OUT_Z_LSB_REG_ADDRESS						(0x06)


/* Private variables ---------------------------------------------------------*/
static		float	f_fxas21002_gyro_scale = 31.25;


/**
  * @brief 	init fxas21002 				
  */
void	Init_Fxas21002(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_fxas21002_reg_config_value_p;
	
	/**
		* CTRL1
		* bit6-RST			1-trig device reset  	0-no action
		* bit5-ST				1-ENBALE							0-DISABLE
		* bit4:2-DR			000-800   800/2^DR_Value
		* bit1-ACTIVE  	
		* bit0-READY		00-standby 01-ready 1x-active
		*/
	uc_fxas21002_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	HAL_Delay(70);
	
	
	/**
		* CTRL0
		* bit7:6-BW			LPF截止频率  	ODR800HZ(00-256HZ 01-128HZ 1x-64HZ)
		* bit5-SPIW			1-SPI 3-wire mode 0-SPI 4-wire mode						
		* bit4:3-SEL		HPF截止频率	 	ODR800HZ(00-15HZ	01-7.7HZ 10-3.9HZ 11-1.98HZ)
		* bit2-HPF_EN		1-HPF ENABLE	0-HPF DISABLE 	
		* bit1:0-FS			00-2000	01-1000	10-500 11-250
		*/
	uc_fxas21002_reg_config_value_p = 0x01;   //standby
	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL0_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	
	//计算陀螺的分辨率
	f_fxas21002_gyro_scale = 0.0625/(0x01<<(uc_fxas21002_reg_config_value_p&0x03));
	
	/**
		* CTRL1
		* bit6-RST			1-trig device reset  	0-no action
		* bit5-ST				1-ENBALE							0-DISABLE
		* bit4:2-DR			000-800   800/2^DR_Value
		* bit1-ACTIVE  	
		* bit0-READY		00-standby 01-ready 1x-active
		*/
	uc_fxas21002_reg_config_value_p = 0x02;   //active
	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	HAL_Delay(70);     //standby to active delay
	
}
	


/**
  * @brief 	get fxas21002 gyro data
	* @note	 	不要再其他位置访问fxas21002
	*					角速度数据为float类型，存储在float *	f_fxas21002_gyro_p (float	fxas21002_gyro[3])
  */

void	Read_Fxas21002_Gyro_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_fxas21002_gyro_data_p)
{
	static	FlagStatus	st_first_read_fxas21002_reg_data_flag_p = SET;
	
	uint8_t		uc_fxas21002_gyro_reg_data_p[7];
	
	if(SET == st_first_read_fxas21002_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		HAL_I2C_Mem_Read(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_fxas21002_gyro_reg_data_p,	7,	100);	

		st_first_read_fxas21002_reg_data_flag_p = RESET;
	}
	else
	{
//		HAL_I2C_Master_Receive(hi2c,	FXAS21002_DEVICE_ADDRESS,	uc_fxas21002_gyro_reg_data_p,	7,	10);	
		HAL_I2C_Mem_Read(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_fxas21002_gyro_reg_data_p,	7,	100);	
	}
	
	p_f_fxas21002_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[1] <<8 )|uc_fxas21002_gyro_reg_data_p[2] )*f_fxas21002_gyro_scale;
	p_f_fxas21002_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[3] <<8 )|uc_fxas21002_gyro_reg_data_p[4] )*f_fxas21002_gyro_scale;
	p_f_fxas21002_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[5] <<8 )|uc_fxas21002_gyro_reg_data_p[6] )*f_fxas21002_gyro_scale;	
}


//---------------------------------------test  soft iic读取--------------------------------------------//
/**
  * @brief 	init fxas21002 				
  */
uint8_t	uc_fxas21002_id;  //test
void	Init_Fxas21002_Soft(void)
{
	uint8_t	uc_fxas21002_reg_config_value_p;
	
	uc_fxas21002_id = I2C_Read_Single_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_ID_REG_ADDRESS);
	/**
		* CTRL1
		* bit6-RST			1-trig device reset  	0-no action
		* bit5-ST				1-ENBALE							0-DISABLE
		* bit4:2-DR			000-800   800/2^DR_Value
		* bit1-ACTIVE  	
		* bit0-READY		00-standby 01-ready 1x-active
		*/
	uc_fxas21002_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_CTRL1_REG_ADDRESS,uc_fxas21002_reg_config_value_p);
	HAL_Delay(70);
	
	
	/**
		* CTRL0
		* bit7:6-BW			LPF截止频率  	ODR800HZ(00-256HZ 01-128HZ 1x-64HZ)
		* bit5-SPIW			1-SPI 3-wire mode 0-SPI 4-wire mode						
		* bit4:3-SEL		HPF截止频率	 	ODR800HZ(00-15HZ	01-7.7HZ 10-3.9HZ 11-1.98HZ)
		* bit2-HPF_EN		1-HPF ENABLE	0-HPF DISABLE 	
		* bit1:0-FS			00-2000	01-1000	10-500 11-250
		*/
	uc_fxas21002_reg_config_value_p = 0x80;   //standby
//	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL0_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_CTRL0_REG_ADDRESS,uc_fxas21002_reg_config_value_p);
	
	//计算陀螺的分辨率
	f_fxas21002_gyro_scale = 0.0625/(0x01<<(uc_fxas21002_reg_config_value_p&0x03));
	
	/**
		* CTRL1
		* bit6-RST			1-trig device reset  	0-no action
		* bit5-ST				1-ENBALE							0-DISABLE
		* bit4:2-DR			000-800   800/2^DR_Value
		* bit1-ACTIVE  	
		* bit0-READY		00-standby 01-ready 1x-active
		*/
	uc_fxas21002_reg_config_value_p = 0x02;   //active
//	HAL_I2C_Mem_Write(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_fxas21002_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_CTRL1_REG_ADDRESS,uc_fxas21002_reg_config_value_p);
	HAL_Delay(70);     //standby to active delay
	
}
	


/**
  * @brief 	get fxas21002 gyro data
	* @note	 	不要再其他位置访问fxas21002
	*					角速度数据为float类型，存储在float *	f_fxas21002_gyro_p (float	fxas21002_gyro[3])
  */

void	Read_Fxas21002_Gyro_Data_Soft(float *	p_f_fxas21002_gyro_data_p)
{
	static	FlagStatus	st_first_read_fxas21002_reg_data_flag_p = SET;
	
	uint8_t		uc_fxas21002_gyro_reg_data_p[7];
	
	if(SET == st_first_read_fxas21002_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
//		HAL_I2C_Mem_Read(hi2c,	FXAS21002_DEVICE_ADDRESS,	FXAS21002_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_fxas21002_gyro_reg_data_p,	7,	100);	
		I2C_Read_Multi_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_STATUS_REG_ADDRESS,uc_fxas21002_gyro_reg_data_p,7);
		st_first_read_fxas21002_reg_data_flag_p = RESET;
	}
	else
	{
//		HAL_I2C_Master_Receive(hi2c,	FXAS21002_DEVICE_ADDRESS,	uc_fxas21002_gyro_reg_data_p,	7,	10);	
		I2C_Read_Multi_Reg(FXAS21002_DEVICE_ADDRESS,FXAS21002_STATUS_REG_ADDRESS,uc_fxas21002_gyro_reg_data_p,7);
	}
	
	p_f_fxas21002_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[1] <<8 )|uc_fxas21002_gyro_reg_data_p[2] )*f_fxas21002_gyro_scale;
	p_f_fxas21002_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[3] <<8 )|uc_fxas21002_gyro_reg_data_p[4] )*f_fxas21002_gyro_scale;
	p_f_fxas21002_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_fxas21002_gyro_reg_data_p[5] <<8 )|uc_fxas21002_gyro_reg_data_p[6] )*f_fxas21002_gyro_scale;	
}
