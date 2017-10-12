/**
  ******************************************************************************
  * File Name          : mma8452_driver.c
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
	
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "mma8452_driver.h"

#include "soft_iic.h"
/* Private define ------------------------------------------------------------*/
/** @defgroup MMA8452 Device And Register Address
  
  */

#define		MMA8452_DEVICE_ADDRESS									(0x3A)  //0x38@SA0=0	0x3A@SA0=1

/**	Bit7:4-ZYXOW ZOW YOW XOW	(1-overwrite 		0-no overwrite)
	*	BIT3:0-ZYXDR ZDR YDE XDR	(1-data ready		0-no data ready)
	*	
	*/
#define		MMA8452_STATUS_REG_ADDRESS							(0x00)

/**	MSB:bit7-bit0��Ӧ(DATA11-DATA4)
	*	LSB:bit7-bit4��Ӧ(DATA3 -DATA0) bit3-bit0ȫ��Ϊ0
	*/
#define		MMA8452_OUT_X_MSB_REG_ADDRESS						(0x01)
#define		MMA8452_OUT_X_LSB_REG_ADDRESS						(0x02)
#define		MMA8452_OUT_Y_MSB_REG_ADDRESS						(0x03)
#define		MMA8452_OUT_Y_LSB_REG_ADDRESS						(0x04)
#define		MMA8452_OUT_Z_MSB_REG_ADDRESS						(0x05)
#define		MMA8452_OUT_Z_LSB_REG_ADDRESS						(0x06)

/**	indicate mma8452 current operating mode(ֻ��ָʾ״̬����������)
	*	bit1:SYSMOD1	bit0:SYSMOD0
	* 00:STANDBY MODE		01:WAKE MODE	10:SLEEP MODE
	*/
#define		MMA8452_SYSMODE_REG_ADDRESS							(0x0B)


/**	�ж�Դ ����μ�MMA8452�����ֲ�
	*/
#define		MMA8452_INT_SRC_REG_ADDRESS							(0x0C)


/**	who am i register mma8452 id = 0x2A
	*/
#define		MMA8452_ID_REG_ADDRESS									(0x0D)


/**	XYZ_DATA_CONFIG �Ĵ���������FS��HPF����
	*	Bit4:HPF_OUT 		1-Enable HPF  0-DISABLE HPF
	* Bit1:0-FS1FS0		00-2g	01-4g 10-8g
	*/
#define		MMA8452_XYZ_DATA_CFG_REG_ADDRESS				(0x0E)

/**	HPF_CUT_OFF�Ĵ���������HPF��ֹƵ��
	*	Bit5:Pulse_HPF_BYP	0-HPF enabled for pulse processing	1-HPF disable for pulse processing
	* Bit4:Pulse_LPF_EN		0-LPF disbale for pulse processing	1-LPF enabled for pulse processing
	* Bit1:0-SEL[1:0](HPF CUTOFF)			00-16HZ 01-8HZ 10-4HZ 11-2HZ(@ODR-800HZ oversampling mode=normal���������Datasheet)
	*/
#define		MMA8452_HPF_CUT_OFF_REG_ADDRESS					(0x0F)


/**	Bit7:6-ASLP_RATE[1:0] ������sleepģʽ��auto_wake����Ƶ��
	* Bit5:3-DR[2:0]	000-800Hz 001-400Hz 010-200Hz 011-100Hz 100-50HZ 101-12.5Hz 110-6.25Hz 111-1.56Hz
	* Bit2:LNOISE			0-Normal mode 	1-reduced noise mode
	* Bit1:F_READ			0-Normal mode 	1-Fast Read Mode �Զ�ָ�������ݼĴ���δ֪�������Ϊ2���������ݵ�LSB(��4Bite)
	* Bit0:ACTIVE			0-STANDBY mode	1-ACTIVE mode
	*/
#define		MMA8452_CTRL1_REG_ADDRESS								(0x2A)


/**	Bit7:ST						0-Self_Test Disable		1-Self_Test Enable
	* Bit6:RST					0-RST Disable					1-RST Enbale
	* Bit4:3-SMODS[1:0]	00-Normal 01-Low Noise Low Power 10-High Resolution	11-Low Power			
	* Bit2:SLPE					0-AutoSleep Disable		1-AutoSleep Enbale
	* Bit1:0-MODS[1:0]	��SMODS[1:0]��ͬ
	*/
#define		MMA8452_CTRL2_REG_ADDRESS								(0x2B)

/**	Interrupt Control Register
	* CTRL3��������Щ�¼����Ի������� �����ж�IO���Ժ������ʽ(��© ����)
	* CTRL4: �����ж�ʹ��
	* CTRL5: �����ж�Դ��IO�ڵ�ӳ���ϵ
	*/
#define		MMA8452_CTRL3_REG_ADDRESS								(0x2C)
#define		MMA8452_CTRL4_REG_ADDRESS								(0x2D)
#define		MMA8452_CTRL5_REG_ADDRESS								(0x2E)

/**	 OFFSET correction register
	*/
#define		MMA8452_OFFSET_X_CORRECTION_REG_ADDRESS	(0x2F)
#define		MMA8452_OFFSET_Y_CORRECTION_REG_ADDRESS	(0x30)
#define		MMA8452_OFFSET_Z_CORRECTION_REG_ADDRESS	(0x31)



/* Private variables ---------------------------------------------------------*/
static		float	f_mma8452_scale = 1/1024.f;



/**
  * @brief 	init fxas21002 				
  */
void	Init_Mma8452(I2C_HandleTypeDef *hi2c)
{
	uint8_t	uc_mma8452_reg_config_value_p;
	
	/**	XYZ_DATA_CONFIG �Ĵ���������FS��HPF����
	*	Bit4:HPF_OUT 		1-Enable HPF  0-DISABLE HPF
	* Bit1:0-FS1FS0		00-2g	01-4g 10-8g
	*/
	uc_mma8452_reg_config_value_p = 0x00;   //2g
	HAL_I2C_Mem_Write(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_XYZ_DATA_CFG_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_mma8452_reg_config_value_p,	1,	100);
	f_mma8452_scale = (1/1024.f)*( 0x01<<(uc_mma8452_reg_config_value_p&0x03) );
	
	
	/**	Bit7:6-ASLP_RATE[1:0] ������sleepģʽ��auto_wake����Ƶ��
	* Bit5:3-DR[2:0]	000-800Hz 001-400Hz 010-200Hz 011-100Hz 100-50HZ 101-12.5Hz 110-6.25Hz 111-1.56Hz
	* Bit2:LNOISE			0-Normal mode 	1-reduced noise mode
	* Bit1:F_READ			0-Normal mode 	1-Fast Read Mode �Զ�ָ�������ݼĴ���δ֪�������Ϊ2���������ݵ�LSB(��4Bite)
	* Bit0:ACTIVE			0-STANDBY mode	1-ACTIVE mode
	*/
	uc_mma8452_reg_config_value_p = 0x01;   //standby
	HAL_I2C_Mem_Write(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_mma8452_reg_config_value_p,	1,	100);
	HAL_Delay(50);
}



/**
  * @brief 	get mma8452 acc data
	* @note	 	��Ҫ������λ�÷���mma8452
	*					���ٶ�����Ϊfloat���ͣ��洢��float *	f_mma8452_acc_p (float	f_mma8452_acc_p[3])
  */

void	Read_Mma8452_Acc_Data(I2C_HandleTypeDef *hi2c,	float *	p_f_mma8452_acc_data_p)
{
	static	FlagStatus	st_first_read_mma8452_reg_data_flag_p = SET;
	
	uint8_t		uc_mma8452_acc_reg_data_p[7];
	
	if(SET == st_first_read_mma8452_reg_data_flag_p)
	{
		/*��ȡһ�νǶȼĴ�����ֵ��ʹauto_increment pointer������angle_reg,��֤������ȡ����Ҫ�����ڲ��Ĵ�����ַ*/
		HAL_I2C_Mem_Read(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_mma8452_acc_reg_data_p,	7,	100);	
				
		st_first_read_mma8452_reg_data_flag_p = RESET;
	}
	else
	{
//		HAL_I2C_Master_Receive(hi2c,	MMA8452_DEVICE_ADDRESS,	uc_mma8452_acc_reg_data_p,	7,	10);
		HAL_I2C_Mem_Read(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_mma8452_acc_reg_data_p,	7,	100);	
	}
	
	/*ps10 -1 1 -1   ps10 test -1 -1 -1*/
	p_f_mma8452_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[1]<<4 )| (uc_mma8452_acc_reg_data_p[2] >>4) )*f_mma8452_scale;
	p_f_mma8452_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[3]<<4 )| (uc_mma8452_acc_reg_data_p[4] >>4) )*f_mma8452_scale;
	p_f_mma8452_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[5]<<4 )| (uc_mma8452_acc_reg_data_p[6] >>4) )*f_mma8452_scale;
}



//---------------------------------------test  soft iic��ȡ--------------------------------------------//
/**
  * @brief 	init fxas21002 				
  */
uint8_t	uc_mma8452_id;  //test
void	Init_Mma8452_Soft(void)
{
	uint8_t	uc_mma8452_reg_config_value_p;
	
	uc_mma8452_id = I2C_Read_Single_Reg(MMA8452_DEVICE_ADDRESS,MMA8452_ID_REG_ADDRESS);
	/**	XYZ_DATA_CONFIG �Ĵ���������FS��HPF����
	*	Bit4:HPF_OUT 		1-Enable HPF  0-DISABLE HPF
	* Bit1:0-FS1FS0		00-2g	01-4g 10-8g
	*/
	uc_mma8452_reg_config_value_p = 0x00;   //2g
//	HAL_I2C_Mem_Write(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_XYZ_DATA_CFG_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_mma8452_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MMA8452_DEVICE_ADDRESS,MMA8452_XYZ_DATA_CFG_REG_ADDRESS,uc_mma8452_reg_config_value_p);
	f_mma8452_scale = (1/1024.f)*( 0x01<<(uc_mma8452_reg_config_value_p&0x03) );
	
	
	/**	Bit7:6-ASLP_RATE[1:0] ������sleepģʽ��auto_wake����Ƶ��
	* Bit5:3-DR[2:0]	000-800Hz 001-400Hz 010-200Hz 011-100Hz 100-50HZ 101-12.5Hz 110-6.25Hz 111-1.56Hz
	* Bit2:LNOISE			0-Normal mode 	1-reduced noise mode
	* Bit1:F_READ			0-Normal mode 	1-Fast Read Mode �Զ�ָ�������ݼĴ���δ֪�������Ϊ2���������ݵ�LSB(��4Bite)
	* Bit0:ACTIVE			0-STANDBY mode	1-ACTIVE mode
	*/
	uc_mma8452_reg_config_value_p = 0x01;   //standby
//	HAL_I2C_Mem_Write(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_CTRL1_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	&uc_mma8452_reg_config_value_p,	1,	100);
	I2C_Write_Single_Reg(MMA8452_DEVICE_ADDRESS,MMA8452_CTRL1_REG_ADDRESS,uc_mma8452_reg_config_value_p);
	HAL_Delay(50);
}
	

/**
  * @brief 	get mma8452 acc data
	* @note	 	��Ҫ������λ�÷���mma8452
	*					���ٶ�����Ϊfloat���ͣ��洢��float *	f_mma8452_acc_p (float	f_mma8452_acc_p[3])
  */

void	Read_Mma8452_Acc_Data_Soft(float *	p_f_mma8452_acc_data_p)
{
	static	FlagStatus	st_first_read_mma8452_reg_data_flag_p = SET;
	
	uint8_t		uc_mma8452_acc_reg_data_p[7];
	
	if(SET == st_first_read_mma8452_reg_data_flag_p)
	{
		/*��ȡһ�νǶȼĴ�����ֵ��ʹauto_increment pointer������angle_reg,��֤������ȡ����Ҫ�����ڲ��Ĵ�����ַ*/
//		HAL_I2C_Mem_Read(hi2c,	MMA8452_DEVICE_ADDRESS,	MMA8452_STATUS_REG_ADDRESS,	I2C_MEMADD_SIZE_8BIT,	uc_mma8452_acc_reg_data_p,	7,	100);	
		I2C_Read_Multi_Reg(MMA8452_DEVICE_ADDRESS,MMA8452_STATUS_REG_ADDRESS,uc_mma8452_acc_reg_data_p,7);		
		st_first_read_mma8452_reg_data_flag_p = RESET;
	}
	else
	{
//		HAL_I2C_Master_Receive(hi2c,	MMA8452_DEVICE_ADDRESS,	uc_mma8452_acc_reg_data_p,	7,	10);
		I2C_Read_Multi_Reg(MMA8452_DEVICE_ADDRESS,MMA8452_STATUS_REG_ADDRESS,uc_mma8452_acc_reg_data_p,7);	
	}
	
	/*ps10 -1 1 -1   ps10 test -1 -1 -1*/
	p_f_mma8452_acc_data_p[0] = -(float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[1]<<4 )| (uc_mma8452_acc_reg_data_p[2] >>4) )*f_mma8452_scale;
	p_f_mma8452_acc_data_p[1] = (float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[3]<<4 )| (uc_mma8452_acc_reg_data_p[4] >>4) )*f_mma8452_scale;
	p_f_mma8452_acc_data_p[2] = -(float)( ( (int16_t)(int8_t)uc_mma8452_acc_reg_data_p[5]<<4 )| (uc_mma8452_acc_reg_data_p[6] >>4) )*f_mma8452_scale;
}
