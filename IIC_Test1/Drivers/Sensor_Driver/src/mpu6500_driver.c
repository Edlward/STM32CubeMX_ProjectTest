/**
  ******************************************************************************
  * File Name          : mpu6500_driver.c
  * Description        : This file provides code for the mpu6500 .
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
#include "mpu6500_driver.h"
#include "soft_iic.h"

/* Private define ------------------------------------------------------------*/
/** @defgroup MPU65006050 Device And Register Address
  
*/
#define MPU6500_DEVICE_ADDRESS        0xD0    	//0xD0@AD0=0    0xD2@AD0=1


// Experimental DMP support
#define MPU6500_RA_SELF_TEST_X_GYRO     0x00
#define MPU6500_RA_SELF_TEST_Y_GYRO     0x01
#define MPU6500_RA_SELF_TEST_Z_GYRO     0x02

#define MPU6500_RA_SELF_TEST_X_ACCEL    0x0D
#define MPU6500_RA_SELF_TEST_Y_ACCEL    0x0E
#define MPU6500_RA_SELF_TEST_Z_ACCEL    0x0F

#define MPU6500_RA_XG_OFFSET_H          0X13  
#define MPU6500_RA_XG_OFFSET_L          0X14
#define MPU6500_RA_YG_OFFSET_H          0X15
#define MPU6500_RA_YG_OFFSET_L          0X16
#define MPU6500_RA_ZG_OFFSET_H          0X17
#define MPU6500_RA_ZG_OFFSET_L          0X18

#define MPU6500_RA_SMPLRT_DIV           0X19
#define MPU6500_RA_CONFIG               0X1A
#define MPU6500_RA_GYRO_CONFIG          0X1B
#define MPU6500_RA_ACCEL_CONFIG         0X1C
#define MPU6500_RA_ACCEL_CONFIG2        0X1D
#define MPU6500_RA_LP_ACCEL_ODR         0X1E
#define MPU6500_RA_WOM_THR              0X1F

#define MPU6500_RA_FIFO_EN              0X23
#define MPU6500_RA_I2C_MST_CTRL         0X24
#define MPU6500_RA_I2C_SLV0_ADDR        0X25
#define MPU6500_RA_I2C_SLV0_REG         0X26
#define MPU6500_RA_I2C_SLV0_CTRL        0X27
#define MPU6500_RA_I2C_SLV1_ADDR        0X28
#define MPU6500_RA_I2C_SLV1_REG         0X29
#define MPU6500_RA_I2C_SLV1_CTRL        0X2A
#define MPU6500_RA_I2C_SLV2_ADDR        0X2B
#define MPU6500_RA_I2C_SLV2_REG         0X2C
#define MPU6500_RA_I2C_SLV2_CTRL        0X2D
#define MPU6500_RA_I2C_SLV3_ADDR        0X2E
#define MPU6500_RA_I2C_SLV3_REG         0X2F
#define MPU6500_RA_I2C_SLV3_CTRL        0X30
#define MPU6500_RA_I2C_SLV4_ADDR        0X31
#define MPU6500_RA_I2C_SLV4_REG         0X32
#define MPU6500_RA_I2C_SLV4_DO          0X33
#define MPU6500_RA_I2C_SLV4_CTRL        0X34
#define MPU6500_RA_I2C_SLV4_DI          0X35
#define MPU6500_RA_I2C_MST_STATUS       0X36
#define MPU6500_RA_INT_PIN_CFG          0X37
#define MPU6500_RA_INT_ENABLE           0X38
#define MPU6500_RA_INT_STATUS           0X3A
#define MPU6500_RA_ACCEL_XOUT_H         0X3B
#define MPU6500_RA_ACCEL_XOUT_L         0X3C
#define MPU6500_RA_ACCEL_YOUT_H         0X3D
#define MPU6500_RA_ACCEL_YOUT_L         0X3E
#define MPU6500_RA_ACCEL_ZOUT_H         0X3F
#define MPU6500_RA_ACCEL_ZOUT_L         0X40
#define MPU6500_RA_TEMP_OUT_H           0X41
#define MPU6500_RA_TEMP_OUT_L           0X42
#define MPU6500_RA_GYRO_XOUT_H          0X43
#define MPU6500_RA_GYRO_XOUT_L          0X44
#define MPU6500_RA_GYRO_YOUT_H          0X45
#define MPU6500_RA_GYRO_YOUT_L          0X46
#define MPU6500_RA_GYRO_ZOUT_H          0X47
#define MPU6500_RA_GYRO_ZOUT_L          0X48
#define MPU6500_RA_EXT_SENS_DATA_00     0X49
#define MPU6500_RA_EXT_SENS_DATA_01     0X4A
#define MPU6500_RA_EXT_SENS_DATA_02     0X4B
#define MPU6500_RA_EXT_SENS_DATA_03     0X4C
#define MPU6500_RA_EXT_SENS_DATA_04     0X4D
#define MPU6500_RA_EXT_SENS_DATA_05     0X4E
#define MPU6500_RA_EXT_SENS_DATA_06     0X4F
#define MPU6500_RA_EXT_SENS_DATA_07     0X50
#define MPU6500_RA_EXT_SENS_DATA_08     0X51
#define MPU6500_RA_EXT_SENS_DATA_09     0X52
#define MPU6500_RA_EXT_SENS_DATA_10     0X53
#define MPU6500_RA_EXT_SENS_DATA_11     0X54
#define MPU6500_RA_EXT_SENS_DATA_12     0X55
#define MPU6500_RA_EXT_SENS_DATA_13     0X56
#define MPU6500_RA_EXT_SENS_DATA_14     0X57
#define MPU6500_RA_EXT_SENS_DATA_15     0X58
#define MPU6500_RA_EXT_SENS_DATA_16     0X59
#define MPU6500_RA_EXT_SENS_DATA_17     0X5A
#define MPU6500_RA_EXT_SENS_DATA_18     0X5B
#define MPU6500_RA_EXT_SENS_DATA_19     0X5C
#define MPU6500_RA_EXT_SENS_DATA_20     0X5D
#define MPU6500_RA_EXT_SENS_DATA_21     0X5E
#define MPU6500_RA_EXT_SENS_DATA_22     0X5F
#define MPU6500_RA_EXT_SENS_DATA_23     0X60
#define MPU6500_RA_I2C_SLV0_DO          0X63
#define MPU6500_RA_I2C_SLV1_DO          0X64  
#define MPU6500_RA_I2C_SLV2_DO          0X65
#define MPU6500_RA_I2C_SLV3_DO          0X66
#define MPU6500_RA_I2C_MST_DELAY_CTRL   0X67
#define MPU6500_RA_SIGNAL_PATH_RESET    0X68
#define MPU6500_RA_ACCEL_INTEL_CTRL     0X69
#define MPU6500_RA_USER_CTRL            0X6A
#define MPU6500_RA_PWR_MGMT_1           0X6B
#define MPU6500_RA_PWR_MGMT_2           0X6C
#define MPU6500_RA_FIFO_COUNT_H         0X72
#define MPU6500_RA_FIFO_COUNT_L         0X73
#define MPU6500_RA_FIFO_R_W             0X74
#define MPU6500_RA_WHO_AM_I             0X75
#define MPU6500_RA_XA_OFFSET_H          0X77
#define MPU6500_RA_XA_OFFSET_L          0X78
#define MPU6500_RA_YA_OFFSET_H          0X7A
#define MPU6500_RA_YA_OFFSET_L          0X7B
#define MPU6500_RA_ZA_OFFSET_H          0X7D
#define MPU6500_RA_ZA_OFFSET_L          0X7E


/* Private variables ---------------------------------------------------------*/
static		float	f_mpu6500_gyro_scale = 1/16.4f;    	//default 2000dps
static		float	f_mpu6500_acc_scale = 1/16384.f;		//default ±2g


/**
  * @brief 	init mpu6500			
  */
void	Init_Mpu6500(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_mpu6500_reg_config_value_p;
	uint8_t	uc_mpu6500_reg_value_fdb_p;
	
	/**
		* ICM20602_PWR_MGMT_1_REG_ADDR
		* bit7-RST			1-trig device reset  	0-no action
		* bit6-SLEEP		1-SLEEP								0-NORMAL
		* bit5-CYCLE		1-ENABLE							0-DISABLE 周期采样ACC
		* bit4-GYRO_STANDBY	 	1-GYRO_SLEEP  	0-GYRO_NROMAL
		* bit3-TMEP_DIS	1-DISBALE TEMP				0-ENABLE_TEMP
		* bit2:0-CLKSEL[2:0]	0or6-internal 20M  1~5-auto clk 7-no clk
		*/
	uc_mpu6500_reg_config_value_p = 0x80;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
	/*wait ICM20602 RESET completed*/
	do
	{
		HAL_I2C_Mem_Read(hi2c, MPU6500_DEVICE_ADDRESS, MPU6500_RA_PWR_MGMT_1, I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_value_fdb_p, 1, 100);
	}while(uc_mpu6500_reg_value_fdb_p&0x80);	
	HAL_Delay(100);
	
	
	/*auto clk to achieve full gyroscope performance*/
	uc_mpu6500_reg_config_value_p = 0x01;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);

	
	/**
		* MPU6500_RA_SIGNAL_PATH_RESET
		* bit2-GYRO RESET		1-RESET		0-NO Action
		* bit1-ACC REST			1-RESET		0-NO Action
		* bit0-TEMP_RESET		1-RESET		0-NO Action
		*/
	uc_mpu6500_reg_config_value_p = 0x07;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_SIGNAL_PATH_RESET,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
	HAL_Delay(100);
	
	
	
	/**
		* MPU6500_RA_USER_CTRL
		* bit7-DMP_EN							1-ENABLE DMP 			0-DISABLE DMP
		* bit6-FIFO_EN						1-ENABLE FIFO 		0-DISABLE FIFO
		* bit5-I2C_MST_EN					1-ENABLE I2C 			0-DISABLE I2C
		* bit4-I2C_IF_DIS					1-SPI Only 				0-SPI
		* bit3-DMP_RST						bit2-FIFO_RST			bit1-I2C_MST_RST		bit0-SIG_COND_RST				
		*/
	uc_mpu6500_reg_config_value_p = 0x20;   //Enable I2C
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_USER_CTRL,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
	
	
	/**
		* MPU6500_RA_CONFIG
		* bit6-FIFO MODE					1-FIFO满了不再填写		0-FIFO满了替换
		* bit5:3-EXT_SYNC_SET			1～7-SYNC SOURCE			0-DISABLE 
		* bit2:0-DLPF_CFG[2:0]		DLPF截至频率配置
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);


	/**
		* ICM20602_GYRO_CFG_REG_ADDR
		* bit7-XG_ST							X_GYRO self-test
		* bit6-YG_ST							Y_GYRO self-test
		* bit5-ZG_ST							Z_GYRO self-test
		* bit4:3-FS_SEL						00-±250dps	01-±500dps	10-±1000dps  11-±2000dps
		* bit1:0-FCHOICE_B				1～3-DLPF DISBALE      	0-DLPF ENABLE
		*/
	uc_mpu6500_reg_config_value_p = 0x18;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_GYRO_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
	
	/*计算陀螺的分辨率*/
	f_mpu6500_gyro_scale = (1/131.f)*(0x01<<( (uc_mpu6500_reg_config_value_p&0x18)>>3 ));
	
	/**
		* MPU6500_RA_ACCEL_CONFIG
		* bit7-XA_ST							X_ACC self-test
		* bit6-YA_ST							Y_ACC self-test
		* bit5-ZA_ST							Z_ACC self-test
		* bit4:3-FS_SEL						00-±2g	01-±4g	10-±8g  11-±16g
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
	
	/*计算陀螺的分辨率*/
	f_mpu6500_acc_scale = (1/16384.f)*(0x01<<( (uc_mpu6500_reg_config_value_p&0x18)>>3 ));


	/**
		* MPU6500_RA_ACCEL_CONFIG
		* bit3-ACC_FCHOICE_B			1-ACC DLPF DISBALE      	0-ACC DLPF ENABLE
		* bit2:0-ACC_DLPF_CFG			0～7-ref datasheet
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	HAL_I2C_Mem_Write(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_CONFIG2,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6500_reg_config_value_p,	1,	100);
}



/**
  * @brief 	get mpu6500 data
	* @note	 	不要再其他位置访问mpu6500
	*					角速度数据为float类型，存储在float *	p_f_mpu6500_gyro_data_p (float	mpu6500_gyro[3])
  */

void	Read_Mpu6500_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_mpu6500_gyro_data_p,	float *p_f_mpu6500_acc_data_p)
{
	static	FlagStatus	st_first_read_mpu6500_reg_data_flag_p = SET;
	
	uint8_t		uc_mpu6500_data_p[14];
	
	if(SET == st_first_read_mpu6500_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		HAL_I2C_Mem_Read(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_XOUT_H,	I2C_MEMADD_SIZE_8BIT,	uc_mpu6500_data_p,	14,	100);	

		st_first_read_mpu6500_reg_data_flag_p = RESET;
	}
	else
	{
		HAL_I2C_Mem_Read(hi2c,	MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_XOUT_H,	I2C_MEMADD_SIZE_8BIT,	uc_mpu6500_data_p,	14,	100);	
	}
	
	p_f_mpu6500_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[0] <<8 )|uc_mpu6500_data_p[1] )*f_mpu6500_acc_scale;
	p_f_mpu6500_acc_data_p[1] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[2] <<8 )|uc_mpu6500_data_p[3] )*f_mpu6500_acc_scale;
	p_f_mpu6500_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[4] <<8 )|uc_mpu6500_data_p[5] )*f_mpu6500_acc_scale;	
	
	p_f_mpu6500_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[8] <<8 )|uc_mpu6500_data_p[9] )*f_mpu6500_gyro_scale;
	p_f_mpu6500_gyro_data_p[1] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[10] <<8 )|uc_mpu6500_data_p[11] )*f_mpu6500_gyro_scale;
	p_f_mpu6500_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[12] <<8 )|uc_mpu6500_data_p[13] )*f_mpu6500_gyro_scale;	
}

/*--------------------------------------------------模拟I2C读取MPU6050数据--------------------------------------*/
/**
  * @brief 	init mpu6050 				
  */
uint8_t	uc_mpu6500_reg_value_fdb_p;  //test
/**
  * @brief 	init mpu6050 				
  */
void	Init_Mpu6500_Soft(void)
{
	uint8_t	uc_mpu6500_reg_config_value_p;
	
	uc_mpu6500_reg_value_fdb_p = I2C_Read_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_WHO_AM_I);
	/**
		* ICM20602_PWR_MGMT_1_REG_ADDR
		* bit7-RST			1-trig device reset  	0-no action
		* bit6-SLEEP		1-SLEEP								0-NORMAL
		* bit5-CYCLE		1-ENABLE							0-DISABLE 周期采样ACC
		* bit4-GYRO_STANDBY	 	1-GYRO_SLEEP  	0-GYRO_NROMAL
		* bit3-TMEP_DIS	1-DISBALE TEMP				0-ENABLE_TEMP
		* bit2:0-CLKSEL[2:0]	0or6-internal 20M  1~5-auto clk 7-no clk
		*/
	uc_mpu6500_reg_config_value_p = 0x80;   //standby
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_PWR_MGMT_1,	uc_mpu6500_reg_config_value_p);
	/*wait ICM20602 RESET completed*/
	while(I2C_Read_Single_Reg(MPU6500_DEVICE_ADDRESS, MPU6500_RA_PWR_MGMT_1)&0x80);
	HAL_Delay(100);
	
	/*auto clk to achieve full gyroscope performance*/
	uc_mpu6500_reg_config_value_p = 0x01;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_PWR_MGMT_1,	uc_mpu6500_reg_config_value_p);

	
	/**
		* MPU6500_RA_SIGNAL_PATH_RESET
		* bit2-GYRO RESET		1-RESET		0-NO Action
		* bit1-ACC REST			1-RESET		0-NO Action
		* bit0-TEMP_RESET		1-RESET		0-NO Action
		*/
	uc_mpu6500_reg_config_value_p = 0x07;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_SIGNAL_PATH_RESET,	uc_mpu6500_reg_config_value_p);
	HAL_Delay(100);
	
	
	
	/**
		* MPU6500_RA_USER_CTRL
		* bit7-DMP_EN							1-ENABLE DMP 			0-DISABLE DMP
		* bit6-FIFO_EN						1-ENABLE FIFO 		0-DISABLE FIFO
		* bit5-I2C_MST_EN					1-ENABLE I2C 			0-DISABLE I2C
		* bit4-I2C_IF_DIS					1-SPI Only 				0-I2C
		* bit3-DMP_RST						bit2-FIFO_RST			bit1-I2C_MST_RST		bit0-SIG_COND_RST				
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   //Enable I2C
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_USER_CTRL,	uc_mpu6500_reg_config_value_p);
	
	
	/**
		* MPU6500_RA_SMPLRT_DIV
		* bit7:0-SMPLRT_DIV[7:0]		SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
		*														where INTERNAL_SAMPLE_RATE = 1kHz
		
	uc_mpu6500_reg_config_value_p = 0x07;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_SMPLRT_DIV,	uc_mpu6500_reg_config_value_p);
	*/
	
	
	/**
		* MPU6500_RA_CONFIG
		* bit6-FIFO MODE					1-FIFO满了不再填写		0-FIFO满了替换
		* bit5:3-EXT_SYNC_SET			1～7-SYNC SOURCE			0-DISABLE 
		* bit2:0-DLPF_CFG[2:0]		DLPF截至频率配置
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_CONFIG,	uc_mpu6500_reg_config_value_p);


	/**
		* ICM20602_GYRO_CFG_REG_ADDR
		* bit7-XG_ST							X_GYRO self-test
		* bit6-YG_ST							Y_GYRO self-test
		* bit5-ZG_ST							Z_GYRO self-test
		* bit4:3-FS_SEL						00-±250dps	01-±500dps	10-±1000dps  11-±2000dps
		* bit1:0-FCHOICE_B				1～3-DLPF DISBALE      	0-DLPF ENABLE
		*/
	uc_mpu6500_reg_config_value_p = 0x18;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_GYRO_CONFIG,	uc_mpu6500_reg_config_value_p);
	
	/*计算陀螺的分辨率*/
	f_mpu6500_gyro_scale = (1/131.f)*(0x01<<( (uc_mpu6500_reg_config_value_p&0x18)>>3 ));
	
	/**
		* MPU6500_RA_ACCEL_CONFIG
		* bit7-XA_ST							X_ACC self-test
		* bit6-YA_ST							Y_ACC self-test
		* bit5-ZA_ST							Z_ACC self-test
		* bit4:3-FS_SEL						00-±2g	01-±4g	10-±8g  11-±16g
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_CONFIG,	uc_mpu6500_reg_config_value_p);
	
	/*计算陀螺的分辨率*/
	f_mpu6500_acc_scale = (1/16384.f)*(0x01<<( (uc_mpu6500_reg_config_value_p&0x18)>>3 ));


	/**
		* MPU6500_RA_ACCEL_CONFIG
		* bit3-ACC_FCHOICE_B			1-ACC DLPF DISBALE      	0-ACC DLPF ENABLE
		* bit2:0-ACC_DLPF_CFG			0～7-ref datasheet
		*/
	uc_mpu6500_reg_config_value_p = 0x00;   
	I2C_Write_Single_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_CONFIG2,	uc_mpu6500_reg_config_value_p);
}


/**
  * @brief 	get mpu6500 data
	* @note	 	不要再其他位置访问mpu6500
	*					角速度数据为float类型，存储在float *	p_f_mpu6500_gyro_data_p (float	mpu6500_gyro[3])
  */
uint8_t		uc_mpu6500_data_p[14];
void	Read_Mpu6500_Data_Soft(float *	p_f_mpu6500_gyro_data_p,	float *p_f_mpu6500_acc_data_p)
{
	static	FlagStatus	st_first_read_mpu6500_reg_data_flag_p = SET;	
	
	if(SET == st_first_read_mpu6500_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		I2C_Read_Multi_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_XOUT_H,	uc_mpu6500_data_p,	14);	
		st_first_read_mpu6500_reg_data_flag_p = RESET;
	}
	else
	{
		I2C_Read_Multi_Reg(MPU6500_DEVICE_ADDRESS,	MPU6500_RA_ACCEL_XOUT_H,	uc_mpu6500_data_p,	14);	
	}
	
	p_f_mpu6500_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[0] <<8 )|uc_mpu6500_data_p[1] )*f_mpu6500_acc_scale;
	p_f_mpu6500_acc_data_p[1] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[2] <<8 )|uc_mpu6500_data_p[3] )*f_mpu6500_acc_scale;
	p_f_mpu6500_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[4] <<8 )|uc_mpu6500_data_p[5] )*f_mpu6500_acc_scale;	
	
	p_f_mpu6500_gyro_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[8] <<8 )|uc_mpu6500_data_p[9] )*f_mpu6500_gyro_scale;
	p_f_mpu6500_gyro_data_p[1] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[10] <<8 )|uc_mpu6500_data_p[11] )*f_mpu6500_gyro_scale;
	p_f_mpu6500_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6500_data_p[12] <<8 )|uc_mpu6500_data_p[13] )*f_mpu6500_gyro_scale;	
}

