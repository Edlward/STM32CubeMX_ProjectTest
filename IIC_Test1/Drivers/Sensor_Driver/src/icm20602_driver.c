/**
  ******************************************************************************
  * File Name          : icm20602_driver.c
  * Description        : This file provides code for the icm_20602 .
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
#include "icm20602_driver.h"

#include "soft_iic.h"



/* Private define ------------------------------------------------------------*/
/** @defgroup ICM20602 Device And Register Address
  
  */

#define		ICM20602_DEVICE_ADDRESS										(0xD0)  //0xD0@SA0=0  0xD2@SA0=1 

#define		ICM20602_ID_REG_ADDRESS										(0x75)  //who am i

/*ICM20602配置寄存器*/
#define 	ICM20602_CFG_REG_ADDR											(0x1A)
#define 	ICM20602_GYRO_CFG_REG_ADDR								(0x1B)
#define 	ICM20602_ACC_CFG_REG_ADDR									(0x1C)
#define 	ICM20602_ACC_CFG2_REG_ADDR								(0x1D)
#define 	ICM20602_GYRO_LPM_CFG_REG_ADDR						(0x1E)


/*ICM20602的数据寄存器*/
#define 	ICM20602_ACC_XOUT_H_REG_ADDR							(0x3B)
#define 	ICM20602_ACC_XOUT_L_REG_ADDR							(0x3C)
#define 	ICM20602_ACC_YOUT_H_REG_ADDR							(0x3D)
#define 	ICM20602_ACC_YOUT_L_REG_ADDR							(0x3E)
#define 	ICM20602_ACC_ZOUT_H_REG_ADDR							(0x3F)
#define 	ICM20602_ACC_ZOUT_L_REG_ADDR							(0x40)

#define 	ICM20602_TMEP_OUT_H_REG_ADDR							(0x41)
#define 	ICM20602_TMEP_OUT_L_REG_ADDR							(0x42)

#define 	ICM20602_GYRO_XOUT_H_REG_ADDR							(0x43)
#define 	ICM20602_GYRO_XOUT_L_REG_ADDR							(0x44)
#define 	ICM20602_GYRO_YOUT_H_REG_ADDR							(0x45)
#define 	ICM20602_GYRO_YOUT_L_REG_ADDR							(0x46)
#define 	ICM20602_GYRO_ZOUT_H_REG_ADDR							(0x47)
#define 	ICM20602_GYRO_ZOUT_L_REG_ADDR							(0x48)

/*ICM20602的电源寄存器*/
#define 	ICM20602_USER_CTRL_REG_ADDR								(0x6A)
#define 	ICM20602_PWR_MGMT_1_REG_ADDR							(0x6B)
#define 	ICM20602_PWR_MGMT_2_REG_ADDR							(0x6C)


/* Private variables ---------------------------------------------------------*/
static		float	f_icm20602_gyro_scale = 1/16.4f;    	//default 2000dps
static		float	f_icm20602_acc_scale = 1/16384.f;		//default ±2g


/**
  * @brief 	init mpu6050 				
  */
void	Init_Icm20602(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_icm20602_reg_config_value_p;
	uint8_t	uc_icm20602_reg_value_fdb_p;
	
	/**
		* ICM20602_PWR_MGMT_1_REG_ADDR
		* bit7-RST			1-trig device reset  	0-no action
		* bit6-SLEEP		1-SLEEP								0-NORMAL
		* bit5-CYCLE		1-ENABLE							0-DISABLE 周期采样ACC
		* bit4-GYRO_STANDBY	 	1-GYRO_SLEEP  	0-GYRO_NROMAL
		* bit3-TMEP_DIS	1-DISBALE TEMP				0-ENABLE_TEMP
		* bit2:0-CLKSEL[2:0]	0or6-internal 20M  1~5-auto clk 7-no clk
		*/
	uc_icm20602_reg_config_value_p = 0x80;   //standby
	HAL_I2C_Mem_Write(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_PWR_MGMT_1_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_config_value_p,	1,	100);
	/*wait ICM20602 RESET completed*/
	do
	{
		HAL_I2C_Mem_Read(hi2c, ICM20602_DEVICE_ADDRESS, ICM20602_PWR_MGMT_1_REG_ADDR, I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_value_fdb_p, 1, 100);
	}while(uc_icm20602_reg_value_fdb_p&0x80);
	HAL_Delay(100);
	
	/*auto clk to achieve full gyroscope performance*/
	uc_icm20602_reg_config_value_p = 0x01;   
	HAL_I2C_Mem_Write(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_PWR_MGMT_1_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_config_value_p,	1,	100);

	/**
		* ICM20602_CFG_REG_ADDR
		* bit6-FIFO MODE					1-FIFO满了不再填写		0-FIFO满了替换
		* bit5:3-EXT_SYNC_SET			1～7-SYNC SOURCE			0-DISABLE 
		* bit2:0-DLPF_CFG[2:0]		DLPF截至频率配置
		*/
	uc_icm20602_reg_config_value_p = 0x00;   
	HAL_I2C_Mem_Write(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_CFG_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_config_value_p,	1,	100);
	
	/**
		* ICM20602_GYRO_CFG_REG_ADDR
		* bit7-XG_ST							X_GYRO self-test
		* bit6-YG_ST							Y_GYRO self-test
		* bit5-ZG_ST							Z_GYRO self-test
		* bit4:3-FS_SEL						00-±250dps	01-±500dps	10-±1000dps  11-±2000dps
		* bit1:0-FCHOICE_B				1～3-DLPF DISBALE      	0-DLPF ENABLE
		*/
	uc_icm20602_reg_config_value_p = 0x18;   
	HAL_I2C_Mem_Write(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_GYRO_CFG_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_config_value_p,	1,	100);
	
	/*计算陀螺的分辨率*/
	f_icm20602_gyro_scale = (1/131.f)*(0x01<<( (uc_icm20602_reg_config_value_p&0x18)>>3 ));
	
	/**
		* ICM20602_ACC_CFG_REG_ADDR
		* bit7-XA_ST							X_ACC self-test
		* bit6-YA_ST							Y_ACC self-test
		* bit5-ZA_ST							Z_ACC self-test
		* bit4:3-FS_SEL						00-±2g	01-±4g	10-±8g  11-±16g
		*/
	uc_icm20602_reg_config_value_p = 0x00;   
	HAL_I2C_Mem_Write(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_CFG_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	&uc_icm20602_reg_config_value_p,	1,	100);
	
	/*计算陀螺的分辨率*/
	f_icm20602_acc_scale = (1/16384.f)*(0x01<<( (uc_icm20602_reg_config_value_p&0x18)>>3 ));
}



/**
  * @brief 	get icm20602 data
	* @note	 	不要再其他位置访问icm20602
	*					角速度数据为float类型，存储在float *	p_f_icm20602_gyro_data_p (float	icm20602_gyro[3])
  */

void	Read_Icm20602_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_icm20602_gyro_data_p,	float *p_f_icm20602_acc_data_p)
{
	static	FlagStatus	st_first_read_icm20602_reg_data_flag_p = SET;
	
	uint8_t		uc_icm20602_data_p[14];
	
	if(SET == st_first_read_icm20602_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		HAL_I2C_Mem_Read(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_XOUT_H_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	uc_icm20602_data_p,	14,	100);	
		st_first_read_icm20602_reg_data_flag_p = RESET;
	}
	else
	{
		HAL_I2C_Mem_Read(hi2c,	ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_XOUT_H_REG_ADDR,	I2C_MEMADD_SIZE_8BIT,	uc_icm20602_data_p,	14,	100);	
	}
	
	p_f_icm20602_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[0] <<8 )|uc_icm20602_data_p[1] )*f_icm20602_acc_scale;
	p_f_icm20602_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_icm20602_data_p[2] <<8 )|uc_icm20602_data_p[3] )*f_icm20602_acc_scale;
	p_f_icm20602_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[4] <<8 )|uc_icm20602_data_p[5] )*f_icm20602_acc_scale;	
	
	p_f_icm20602_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[8] <<8 )|uc_icm20602_data_p[9] )*f_icm20602_gyro_scale;
	p_f_icm20602_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_icm20602_data_p[10] <<8 )|uc_icm20602_data_p[11] )*f_icm20602_gyro_scale;
	p_f_icm20602_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[12] <<8 )|uc_icm20602_data_p[13] )*f_icm20602_gyro_scale;	
}



/*--------------------------------------------------模拟I2C读取Icm20602数据--------------------------------------*/
/**
  * @brief 	init Icm20602		
  */
uint8_t	uc_icm20602_reg_value_fdb_p; 
void	Init_Icm20602_Soft(void)
{
	uint8_t	uc_icm20602_reg_config_value_p;
	
	
	uc_icm20602_reg_value_fdb_p = I2C_Read_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_ID_REG_ADDRESS);
	
	/**
		* ICM20602_PWR_MGMT_1_REG_ADDR
		* bit7-RST			1-trig device reset  	0-no action
		* bit6-SLEEP		1-SLEEP								0-NORMAL
		* bit5-CYCLE		1-ENABLE							0-DISABLE 周期采样ACC
		* bit4-GYRO_STANDBY	 	1-GYRO_SLEEP  	0-GYRO_NROMAL
		* bit3-TMEP_DIS	1-DISBALE TEMP				0-ENABLE_TEMP
		* bit2:0-CLKSEL[2:0]	0or6-internal 20M  1~5-auto clk 7-no clk
		*/
	uc_icm20602_reg_config_value_p = 0x80;   //standby
	I2C_Write_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_PWR_MGMT_1_REG_ADDR, uc_icm20602_reg_config_value_p);
	/*wait ICM20602 RESET completed*/
	while(I2C_Read_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_PWR_MGMT_1_REG_ADDR)&0x80);
	HAL_Delay(100);
	
	/*auto clk to achieve full gyroscope performance*/
	uc_icm20602_reg_config_value_p = 0x01;   
	I2C_Write_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_PWR_MGMT_1_REG_ADDR, uc_icm20602_reg_config_value_p);

	/**
		* ICM20602_CFG_REG_ADDR
		* bit6-FIFO MODE					1-FIFO满了不再填写		0-FIFO满了替换
		* bit5:3-EXT_SYNC_SET			1～7-SYNC SOURCE			0-DISABLE 
		* bit2:0-DLPF_CFG[2:0]		DLPF截至频率配置
		*/
	uc_icm20602_reg_config_value_p = 0x00;   
	I2C_Write_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_CFG_REG_ADDR,	uc_icm20602_reg_config_value_p);
	
	/**
		* ICM20602_GYRO_CFG_REG_ADDR
		* bit7-XG_ST							X_GYRO self-test
		* bit6-YG_ST							Y_GYRO self-test
		* bit5-ZG_ST							Z_GYRO self-test
		* bit4:3-FS_SEL						00-±250dps	01-±500dps	10-±1000dps  11-±2000dps
		* bit1:0-FCHOICE_B				1～3-DLPF DISBALE      	0-DLPF ENABLE
		*/
	uc_icm20602_reg_config_value_p = 0x18;   
	I2C_Write_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_GYRO_CFG_REG_ADDR,	uc_icm20602_reg_config_value_p);
	
	uc_icm20602_reg_value_fdb_p = I2C_Read_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_GYRO_CFG_REG_ADDR);
	/*计算陀螺的分辨率*/
	f_icm20602_gyro_scale = (1/131.f)*(0x01<<( (uc_icm20602_reg_config_value_p&0x18)>>3 ));
	
	/**
		* ICM20602_ACC_CFG_REG_ADDR
		* bit7-XA_ST							X_ACC self-test
		* bit6-YA_ST							Y_ACC self-test
		* bit5-ZA_ST							Z_ACC self-test
		* bit4:3-FS_SEL						00-±2g	01-±4g	10-±8g  11-±16g
		*/
	uc_icm20602_reg_config_value_p = 0x00;   
	I2C_Write_Single_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_CFG_REG_ADDR,	uc_icm20602_reg_config_value_p);
	
	/*计算陀螺的分辨率*/
	f_icm20602_acc_scale = (1/16384.f)*(0x01<<( (uc_icm20602_reg_config_value_p&0x18)>>3 ));
}



/**
  * @brief 	get icm20602 data
	* @note	 	不要再其他位置访问icm20602
	*					角速度数据为float类型，存储在float *	p_f_icm20602_gyro_data_p (float	icm20602_gyro[3])
  */
uint8_t		uc_icm20602_data_p[14];  
float			f_icm20602_temp = 0;
void	Read_Icm20602_Data_Soft(float *	p_f_icm20602_gyro_data_p,	float *p_f_icm20602_acc_data_p)
{
	static	FlagStatus	st_first_read_icm20602_reg_data_flag_p = SET;
	
	
	
	if(SET == st_first_read_icm20602_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		I2C_Read_Multi_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_XOUT_H_REG_ADDR,	uc_icm20602_data_p,	14);	
		st_first_read_icm20602_reg_data_flag_p = RESET;
	}
	else
	{
		I2C_Read_Multi_Reg(ICM20602_DEVICE_ADDRESS,	ICM20602_ACC_XOUT_H_REG_ADDR,	uc_icm20602_data_p,	14);	
	}
	
	p_f_icm20602_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[0] <<8 )|uc_icm20602_data_p[1] )*f_icm20602_acc_scale;
	p_f_icm20602_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_icm20602_data_p[2] <<8 )|uc_icm20602_data_p[3] )*f_icm20602_acc_scale;
	p_f_icm20602_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[4] <<8 )|uc_icm20602_data_p[5] )*f_icm20602_acc_scale;	
	
	p_f_icm20602_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[8] <<8 )|uc_icm20602_data_p[9] )*f_icm20602_gyro_scale;
	p_f_icm20602_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_icm20602_data_p[10] <<8 )|uc_icm20602_data_p[11] )*f_icm20602_gyro_scale;
	p_f_icm20602_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_icm20602_data_p[12] <<8 )|uc_icm20602_data_p[13] )*f_icm20602_gyro_scale;	

	f_icm20602_temp = (float)( ( (int16_t)(int8_t)uc_icm20602_data_p[6] <<8 )|uc_icm20602_data_p[7] )/326.8 + 25;
}
