/**
  ******************************************************************************
  * File Name          : mpu6050_driver.c
  * Description        : This file provides code for the mpu6050 .
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
#include "mpu6050_driver.h"
#include "soft_iic.h"


/* Private define ------------------------------------------------------------*/
/** @defgroup MPU6050 Device And Register Address
  
  */
#define MPU6050_ADDRESS         0x68    //0xD0@AD0=0		0xD2@AD0=1

#define MPU6050_READ_BIT	    (0x01<<7)
#define MPU6050_WRITE_BIT	    (0x00<<7)

// Experimental DMP support
#define DMP_MEM_START_ADDR  0x6E
#define DMP_MEM_R_W 				0x6F

#define INV_MAX_NUM_ACCEL_SAMPLES      (8)
#define DMP_REF_QUATERNION             (0)
#define DMP_REF_GYROS                  (DMP_REF_QUATERNION + 4) // 4
#define DMP_REF_CONTROL                (DMP_REF_GYROS + 3)      // 7
#define DMP_REF_RAW                    (DMP_REF_CONTROL + 4)    // 11
#define DMP_REF_RAW_EXTERNAL           (DMP_REF_RAW + 8)        // 19
#define DMP_REF_ACCEL                  (DMP_REF_RAW_EXTERNAL + 6)       // 25
#define DMP_REF_QUANT_ACCEL            (DMP_REF_ACCEL + 3)      // 28
#define DMP_REF_QUATERNION_6AXIS       (DMP_REF_QUANT_ACCEL + INV_MAX_NUM_ACCEL_SAMPLES)        // 36
#define DMP_REF_EIS                    (DMP_REF_QUATERNION_6AXIS + 4)   // 40
#define DMP_REF_DMP_PACKET             (DMP_REF_EIS + 3)        // 43
#define DMP_REF_GARBAGE                (DMP_REF_DMP_PACKET + 1) // 44
#define DMP_REF_LAST                   (DMP_REF_GARBAGE + 1)    // 45

#define MPU_RA_XG_OFFS_TC       0x00    //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_YG_OFFS_TC       0x01    //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_ZG_OFFS_TC       0x02    //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
#define MPU_RA_X_FINE_GAIN      0x03    //[7:0] X_FINE_GAIN
#define MPU_RA_Y_FINE_GAIN      0x04    //[7:0] Y_FINE_GAIN
#define MPU_RA_Z_FINE_GAIN      0x05    //[7:0] Z_FINE_GAIN
#define MPU_RA_XA_OFFS_H        0x06    //[15:0] XA_OFFS
#define MPU_RA_XA_OFFS_L_TC     0x07
#define MPU_RA_YA_OFFS_H        0x08    //[15:0] YA_OFFS
#define MPU_RA_YA_OFFS_L_TC     0x09
#define MPU_RA_ZA_OFFS_H        0x0A    //[15:0] ZA_OFFS
#define MPU_RA_ZA_OFFS_L_TC     0x0B
#define MPU_RA_PRODUCT_ID       0x0C    // Product ID Register
#define MPU_RA_XG_OFFS_USRH     0x13    //[15:0] XG_OFFS_USR
#define MPU_RA_XG_OFFS_USRL     0x14
#define MPU_RA_YG_OFFS_USRH     0x15    //[15:0] YG_OFFS_USR
#define MPU_RA_YG_OFFS_USRL     0x16
#define MPU_RA_ZG_OFFS_USRH     0x17    //[15:0] ZG_OFFS_USR
#define MPU_RA_ZG_OFFS_USRL     0x18
#define MPU_RA_SMPLRT_DIV       0x19
#define MPU_RA_CONFIG           0x1A
#define MPU_RA_GYRO_CONFIG      0x1B
#define MPU_RA_ACCEL_CONFIG     0x1C
#define MPU_RA_FF_THR           0x1D
#define MPU_RA_FF_DUR           0x1E
#define MPU_RA_MOT_THR          0x1F
#define MPU_RA_MOT_DUR          0x20
#define MPU_RA_ZRMOT_THR        0x21
#define MPU_RA_ZRMOT_DUR        0x22
#define MPU_RA_FIFO_EN          0x23
#define MPU_RA_I2C_MST_CTRL     0x24
#define MPU_RA_I2C_SLV0_ADDR    0x25
#define MPU_RA_I2C_SLV0_REG     0x26
#define MPU_RA_I2C_SLV0_CTRL    0x27
#define MPU_RA_I2C_SLV1_ADDR    0x28
#define MPU_RA_I2C_SLV1_REG     0x29
#define MPU_RA_I2C_SLV1_CTRL    0x2A
#define MPU_RA_I2C_SLV2_ADDR    0x2B
#define MPU_RA_I2C_SLV2_REG     0x2C
#define MPU_RA_I2C_SLV2_CTRL    0x2D
#define MPU_RA_I2C_SLV3_ADDR    0x2E
#define MPU_RA_I2C_SLV3_REG     0x2F
#define MPU_RA_I2C_SLV3_CTRL    0x30
#define MPU_RA_I2C_SLV4_ADDR    0x31
#define MPU_RA_I2C_SLV4_REG     0x32
#define MPU_RA_I2C_SLV4_DO      0x33
#define MPU_RA_I2C_SLV4_CTRL    0x34
#define MPU_RA_I2C_SLV4_DI      0x35
#define MPU_RA_I2C_MST_STATUS   0x36
#define MPU_RA_INT_PIN_CFG      0x37
#define MPU_RA_INT_ENABLE       0x38
#define MPU_RA_DMP_INT_STATUS   0x39
#define MPU_RA_INT_STATUS       0x3A
#define MPU_RA_ACCEL_XOUT_H     0x3B
#define MPU_RA_ACCEL_XOUT_L     0x3C
#define MPU_RA_ACCEL_YOUT_H     0x3D
#define MPU_RA_ACCEL_YOUT_L     0x3E
#define MPU_RA_ACCEL_ZOUT_H     0x3F
#define MPU_RA_ACCEL_ZOUT_L     0x40
#define MPU_RA_TEMP_OUT_H       0x41
#define MPU_RA_TEMP_OUT_L       0x42
#define MPU_RA_GYRO_XOUT_H      0x43
#define MPU_RA_GYRO_XOUT_L      0x44
#define MPU_RA_GYRO_YOUT_H      0x45
#define MPU_RA_GYRO_YOUT_L      0x46
#define MPU_RA_GYRO_ZOUT_H      0x47
#define MPU_RA_GYRO_ZOUT_L      0x48
#define MPU_RA_EXT_SENS_DATA_00 0x49
#define MPU_RA_MOT_DETECT_STATUS    0x61
#define MPU_RA_I2C_SLV0_DO      0x63
#define MPU_RA_I2C_SLV1_DO      0x64
#define MPU_RA_I2C_SLV2_DO      0x65
#define MPU_RA_I2C_SLV3_DO      0x66
#define MPU_RA_I2C_MST_DELAY_CTRL   0x67
#define MPU_RA_SIGNAL_PATH_RESET    0x68
#define MPU_RA_MOT_DETECT_CTRL      0x69
#define MPU_RA_USER_CTRL        0x6A
#define MPU_RA_PWR_MGMT_1       0x6B
#define MPU_RA_PWR_MGMT_2       0x6C
#define MPU_RA_BANK_SEL         0x6D
#define MPU_RA_MEM_START_ADDR   0x6E
#define MPU_RA_MEM_R_W          0x6F
#define MPU_RA_DMP_CFG_1        0x70
#define MPU_RA_DMP_CFG_2        0x71
#define MPU_RA_FIFO_COUNTH      0x72
#define MPU_RA_FIFO_COUNTL      0x73
#define MPU_RA_FIFO_R_W         0x74
#define MPU_RA_WHO_AM_I         0x75

//#define MPU6050_SMPLRT_DIV      0       //8000Hz
//// #define MPU6050_DLPF_CFG        0   // 256Hz
//#define MPU6050_DLPF_CFG   3        // 42Hz

#define MPU6050ES_REV_C4        0x14
#define MPU6050ES_REV_C5        0x15
#define MPU6050ES_REV_D6        0x16
#define MPU6050ES_REV_D7        0x17
#define MPU6050ES_REV_D8        0x18
#define MPU6050_REV_C4          0x54
#define MPU6050_REV_C5          0x55
#define MPU6050_REV_D6          0x56
#define MPU6050_REV_D7          0x57
#define MPU6050_REV_D8          0x58
#define MPU6050_REV_D9          0x59



/* Private variables ---------------------------------------------------------*/
static		float	f_mpu6050_gyro_scale = 1/16.4f;    	//default 2000dps
static		float	f_mpu6050_acc_scale = 1/16384.f;		//default ±2g


/**
  * @brief 	init mpu6050 				
  */
void	Init_Mpu6050(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_mpu6050_reg_config_value_p;
	
	/*WR_MGMT_1  bit7:RESET bit6:SLEEP bit5:CYCLE bit2-bit0:CLK*/
	uc_mpu6050_reg_config_value_p = 0x80;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	HAL_Delay(50);
	
	//WR_MGMT_1  bit7:RESET bit6:SLEEP bit5:CYCLE bit2-bit0:CLK	
	uc_mpu6050_reg_config_value_p = 0x03;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	HAL_Delay(50);
	
	//MPU_RA_PWR_MGMT_2 
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_2,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	
	/*USER_CTRL  bit4:I2C_IF_DIS 使能SPI*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_USER_CTRL,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	
	/*SMPLRT_DIV bit7-bit0:SMPLRT_DIV[7:0]*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_SMPLRT_DIV,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	
	/**
		*INT_PIN_CFG 
		*bit7: INT_LEVEL 0->active high 1->active low
		*bit6: INT_OPEN  0->推挽        1->开漏
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_INT_PIN_CFG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	
	/**
		*INT_ENABLE
		*bit0:DATA_RDY_EN 0->DISABLE 1->ENABLE
		*/
	uc_mpu6050_reg_config_value_p = 0x01;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_INT_ENABLE,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);	
	
	/**
		*CONFIG
		*bit2-bit0	DLPF_CFG	
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);

	/**GYRO_CONFIG
		*bit4-bit3	FS_SEL 00->±250dps	01->±500dps	02->±1000dps 03->±2000dps	
		*/
	uc_mpu6050_reg_config_value_p = 0x18;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_GYRO_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	/*计算陀螺的分辨率*/
	f_mpu6050_gyro_scale = (1/131.f)*(0x01<<( (uc_mpu6050_reg_config_value_p&0x18)>>3 ));
	
	/**ACC_CONFIG
		*bit4-bit3	AFS_SEL	00->±2g	01->±4g	02->±8g 03->±16g
		*bit2-bit0 ACC_HPF 0->NONE	1->5Hz	2->2.5Hz	3->1.25Hz	4->0.63Hz	7->Hold 
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_ACCEL_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	/*计算加速度的分辨率*/
	f_mpu6050_acc_scale = (1/16384.f)*(0x01<<( (uc_mpu6050_reg_config_value_p&0x18)>>3 ));
}



/**
  * @brief 	get mpu6050 data
	* @note	 	不要再其他位置访问fxas21002
	*					角速度数据为float类型，存储在float *	p_f_mpu6050_gyro_data_p (float	p_f_mpu6050_gyro_data_p[3])
	*					加速度数据为float类型，存储在float * 	p_f_mpu6050_acc_data_p (float 	p_f_mpu6050_acc_data_p[3])
  */

void	Read_Mpu6050_Data(I2C_HandleTypeDef *hi2c,float *	p_f_mpu6050_gyro_data_p,	float	*p_f_mpu6050_acc_data_p)
{
	static	FlagStatus	st_first_read_mpu6050_reg_data_flag_p = SET;
	
	uint8_t		uc_mpu6050_reg_data_p[14];
	

	if(SET == st_first_read_mpu6050_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
		HAL_I2C_Mem_Read(hi2c, MPU6050_ADDRESS, MPU_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,	uc_mpu6050_reg_data_p, 14, 100);	
//		I2C_Read_Multi_Reg(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H,uc_mpu6050_reg_data_p,14);
		st_first_read_mpu6050_reg_data_flag_p = RESET;
	}
	else
	{
		HAL_I2C_Mem_Read(hi2c, MPU6050_ADDRESS, MPU_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,	uc_mpu6050_reg_data_p, 14, 100);
//		I2C_Read_Multi_Reg(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H,uc_mpu6050_reg_data_p,14);
	}
	p_f_mpu6050_acc_data_p[0] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[0] <<8 )|uc_mpu6050_reg_data_p[1] )*f_mpu6050_acc_scale;
	p_f_mpu6050_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[2] <<8 )|uc_mpu6050_reg_data_p[3] )*f_mpu6050_acc_scale;
	p_f_mpu6050_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[4] <<8 )|uc_mpu6050_reg_data_p[5] )*f_mpu6050_acc_scale;	
	
	p_f_mpu6050_gyro_data_p[0] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[8] <<8 )|uc_mpu6050_reg_data_p[9] )*f_mpu6050_gyro_scale;
	p_f_mpu6050_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[10] <<8 )|uc_mpu6050_reg_data_p[11] )*f_mpu6050_gyro_scale;
	p_f_mpu6050_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[12] <<8 )|uc_mpu6050_reg_data_p[13] )*f_mpu6050_gyro_scale;	
}



/*-----------------------------------------------模拟I2C读取MPU6050数据-------------------------------*/
/**
  * @brief 	init mpu6050 				
  */
uint8_t		uc_mpu6050_reg_data_fdb;		//test

void	Init_Mpu6050_Soft(void)
{
	uint8_t	uc_mpu6050_reg_config_value_p;
	
	uc_mpu6050_reg_data_fdb = I2C_Read_Single_Reg(MPU6050_ADDRESS,MPU_RA_WHO_AM_I);
	
	/*WR_MGMT_1  bit7:RESET bit6:SLEEP bit5:CYCLE bit2-bit0:CLK*/
	uc_mpu6050_reg_config_value_p = 0x80;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, uc_mpu6050_reg_config_value_p);
	uc_mpu6050_reg_data_fdb = I2C_Read_Single_Reg(MPU6050_ADDRESS,MPU_RA_GYRO_CONFIG);
	HAL_Delay(200);
	
	//WR_MGMT_1  bit7:RESET bit6:SLEEP bit5:CYCLE bit2-bit0:CLK	
	uc_mpu6050_reg_config_value_p = 0x03;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_1,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_1, uc_mpu6050_reg_config_value_p);
	HAL_Delay(50);
	
	//MPU_RA_PWR_MGMT_2 
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_PWR_MGMT_2,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_PWR_MGMT_2, uc_mpu6050_reg_config_value_p);
	
	/*USER_CTRL  bit4:I2C_IF_DIS 使能SPI*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_USER_CTRL,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_USER_CTRL, uc_mpu6050_reg_config_value_p);
	
	/*SMPLRT_DIV bit7-bit0:SMPLRT_DIV[7:0]*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_SMPLRT_DIV,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_SMPLRT_DIV, uc_mpu6050_reg_config_value_p);
	
	/**
		*INT_PIN_CFG 
		*bit7: INT_LEVEL 0->active high 1->active low
		*bit6: INT_OPEN  0->推挽        1->开漏
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_INT_PIN_CFG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_INT_PIN_CFG, uc_mpu6050_reg_config_value_p);
	
	/**
		*INT_ENABLE
		*bit0:DATA_RDY_EN 0->DISABLE 1->ENABLE
		*/
	uc_mpu6050_reg_config_value_p = 0x01;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_INT_ENABLE,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);	
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_INT_ENABLE, uc_mpu6050_reg_config_value_p);
	
	/**
		*CONFIG
		*bit2-bit0	DLPF_CFG	
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_CONFIG, uc_mpu6050_reg_config_value_p);
	
	
	/**GYRO_CONFIG
		*bit4-bit3	FS_SEL 00->±250dps	01->±500dps	02->±1000dps 03->±2000dps	
		*/
	uc_mpu6050_reg_config_value_p = 0x18;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_GYRO_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_GYRO_CONFIG, uc_mpu6050_reg_config_value_p);	
	uc_mpu6050_reg_data_fdb = I2C_Read_Single_Reg(MPU6050_ADDRESS,MPU_RA_GYRO_CONFIG);
	
	/*计算陀螺的分辨率*/
	f_mpu6050_gyro_scale = (1/131.f)*(0x01<<( (uc_mpu6050_reg_config_value_p&0x18)>>3 ));
	
	/**ACC_CONFIG
		*bit4-bit3	AFS_SEL	00->±2g	01->±4g	02->±8g 03->±16g
		*bit2-bit0 ACC_HPF 0->NONE	1->5Hz	2->2.5Hz	3->1.25Hz	4->0.63Hz	7->Hold 
		*/
	uc_mpu6050_reg_config_value_p = 0x00;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MPU6050_ADDRESS,	MPU_RA_ACCEL_CONFIG,	I2C_MEMADD_SIZE_8BIT,	&uc_mpu6050_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MPU6050_ADDRESS, MPU_RA_ACCEL_CONFIG, uc_mpu6050_reg_config_value_p);
	
	/*计算加速度的分辨率*/
	f_mpu6050_acc_scale = (1/16384.f)*(0x01<<( (uc_mpu6050_reg_config_value_p&0x18)>>3 ));
}


/**
  * @brief 	get fxas21002 gyro data
	* @note	 	不要再其他位置访问fxas21002
	*					角速度数据为float类型，存储在float *	p_f_mpu6050_gyro_data_p (float	p_f_mpu6050_gyro_data_p[3])
	*					加速度数据为float类型，存储在float * 	p_f_mpu6050_acc_data_p (float 	p_f_mpu6050_acc_data_p[3])
  */
void	Read_Mpu6050_Data_Soft(float *	p_f_mpu6050_gyro_data_p,	float	*p_f_mpu6050_acc_data_p)
{
	static	FlagStatus	st_first_read_mpu6050_reg_data_flag_p = SET;
	
	uint8_t		uc_mpu6050_reg_data_p[14];
	

	if(SET == st_first_read_mpu6050_reg_data_flag_p)
	{
		/*读取一次角度寄存器的值，使auto_increment pointer锁定在angle_reg,保证后续读取不需要发送内部寄存器地址*/
//		HAL_I2C_Mem_Read(hi2c, MPU6050_ADDRESS, MPU_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,	uc_mpu6050_reg_data_p, 14, 100);	
		I2C_Read_Multi_Reg(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H,uc_mpu6050_reg_data_p,14);
		st_first_read_mpu6050_reg_data_flag_p = RESET;
	}
	else
	{
//		HAL_I2C_Mem_Read(hi2c, MPU6050_ADDRESS, MPU_RA_ACCEL_XOUT_H, I2C_MEMADD_SIZE_8BIT,	uc_mpu6050_reg_data_p, 14, 100);
		I2C_Read_Multi_Reg(MPU6050_ADDRESS,MPU_RA_ACCEL_XOUT_H,uc_mpu6050_reg_data_p,14);
	}
	
	p_f_mpu6050_acc_data_p[0] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[0] <<8 )|uc_mpu6050_reg_data_p[1] )*f_mpu6050_acc_scale;
	p_f_mpu6050_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[2] <<8 )|uc_mpu6050_reg_data_p[3] )*f_mpu6050_acc_scale;
	p_f_mpu6050_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[4] <<8 )|uc_mpu6050_reg_data_p[5] )*f_mpu6050_acc_scale;	
	
//	mpu6000_temp = (w_mpu6000_tmep_sum/MPU6050_DATA_FLT_NUM + 521)/340.0 + 35.0;
	
	p_f_mpu6050_gyro_data_p[0] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[8] <<8 )|uc_mpu6050_reg_data_p[9] )*f_mpu6050_gyro_scale;
	p_f_mpu6050_gyro_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[10] <<8 )|uc_mpu6050_reg_data_p[11] )*f_mpu6050_gyro_scale;
	p_f_mpu6050_gyro_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mpu6050_reg_data_p[12] <<8 )|uc_mpu6050_reg_data_p[13] )*f_mpu6050_gyro_scale;	
}







