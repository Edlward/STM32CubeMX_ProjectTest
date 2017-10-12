#include "stm32f1xx_hal.h"
#include "soft_iic1.h"


#define I2C_SDA1 				GPIO_PIN_11
#define I2C_SCL1 				GPIO_PIN_10	
#define I2C_PORT1   		GPIOB

//
#define I2C_SCL1_0 				HAL_GPIO_WritePin(I2C_PORT1, I2C_SCL1,GPIO_PIN_RESET)
#define I2C_SCL1_1 				HAL_GPIO_WritePin(I2C_PORT1, I2C_SCL1,GPIO_PIN_SET)
#define I2C_SDA1_0 				HAL_GPIO_WritePin(I2C_PORT1, I2C_SDA1,GPIO_PIN_RESET)
#define I2C_SDA1_1   			HAL_GPIO_WritePin(I2C_PORT1, I2C_SDA1,GPIO_PIN_SET)
//
#define I2C_SDA1_STATE   	HAL_GPIO_ReadPin(I2C_PORT1, I2C_SDA1)
#define I2C1_DELAY 				I2C_Delay_1(100000)
#define I2C1_NOP					I2C_Delay_1(1) 		     //更改此NOP的延时时间，可以更改I2C数据读取的快慢，但是要测试可靠性

//
#define I2C1_READY					0x00
#define I2C1_BUS_BUSY				0x01	
#define I2C1_BUS_ERROR			0x02
//
#define I2C1_NACK	  				0x00 
#define I2C1_ACK						0x01


void I2C_Delay_1(unsigned int dly) ;
/*************************************************************************************
函数原型：void I2C_Configuration(void)
函数功能：配置I2C的IO口和初始化I2C
输入参数：无
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_Configuration_1(void)			
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = I2C_SCL1 | I2C_SDA1;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(I2C_PORT1, &GPIO_InitStructure); 
	
		
	I2C_SCL1_1; 												    //初始化I2C电平
	I2C_SDA1_1;
	I2C1_DELAY;
}

/*************************************************************************************
函数原型：void I2C1_DELAY(unsigned int dly)
函数功能：延时函数
输入参数：unsigned int dly：表示延时多少个时钟周期
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_Delay_1(unsigned int dly)               
{


	while(--dly);	//dly=100: 8.75us; dly=100: 85.58 us (SYSCLK=72MHz)
}

/*************************************************************************************
函数原型：unsigned char I2C_START(void)
函数功能：模拟I2C启动时序
输入参数：无
输出参数：无
版本：A1.0
**************************************************************************************/
unsigned char I2C_START_1(void)
{ 
	I2C_SDA1_1; 
 	I2C1_NOP;
  // 
 	I2C_SCL1_1; 
 	I2C1_NOP;    
	//
 	if(!I2C_SDA1_STATE) return I2C1_BUS_BUSY;
	//
 	I2C_SDA1_0;
 	I2C1_NOP;
  //
 	I2C_SCL1_0;  
 	I2C1_NOP; 
	//
 	if(I2C_SDA1_STATE) return I2C1_BUS_ERROR;
	//
 	return I2C1_READY;
}

/*************************************************************************************
函数原型：void I2C_STOP(void)
函数功能：模拟I2C结束时序
输入参数：无
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_STOP_1(void)
{
 	I2C_SDA1_0; 
 	I2C1_NOP;
  // 
 	I2C_SCL1_1; 
 	I2C1_NOP;    
	//
 	I2C_SDA1_1;
 	I2C1_NOP;
}

/*************************************************************************************
函数原型：void I2C_SendACK(void)
函数功能：模拟I2C发送应答(用于主机)
输入参数：无
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_SendACK_1(void)
{
 	I2C_SDA1_0;
 	I2C1_NOP;
 	I2C_SCL1_1;
 	I2C1_NOP;
 	I2C_SCL1_0; 
 	I2C1_NOP;  
}

/*************************************************************************************
函数原型：void I2C_SendNACK(void)
函数功能：模拟I2C不发送应答(用于主机)
输入参数：无
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_SendNACK_1(void)
{
	I2C_SDA1_1;
	I2C1_NOP;
	I2C_SCL1_1;
	I2C1_NOP;
	I2C_SCL1_0; 
	I2C1_NOP;  
}


/*************************************************************************************
函数原型：unsigned char I2C_SendByte(uint8_t i2c_data)
函数功能：模拟I2C发送1byte数据
输入参数：i2c_data：要发送的数据
输出参数：unsigned char：从机有无应答
版本：A1.0
**************************************************************************************/
unsigned char I2C_SendByte_1(uint8_t i2c_data)
{
 	unsigned char i;
 	//
	I2C_SCL1_0;
 	for(i=0;i<8;i++)
 	{  
  		if(i2c_data&0x80) I2C_SDA1_1;
   		else I2C_SDA1_0;
			//
  		i2c_data<<=1;
  		I2C1_NOP;
			//
  		I2C_SCL1_1;
  		I2C1_NOP;
  		I2C_SCL1_0;
  		I2C1_NOP; 
 	}
	//
 	I2C_SDA1_1; 
 	I2C1_NOP;
 	I2C_SCL1_1;
 	I2C1_NOP;   
 	if(I2C_SDA1_STATE)
 	{
  		I2C_SCL1_0;
  		return I2C1_NACK;
 	}
 	else
 	{
  		I2C_SCL1_0;
  		return I2C1_ACK;  
 	}    
}


/*************************************************************************************
函数原型：unsigned char I2C_ReceiveByte_NoACK(void)
函数功能：模拟I2C接收1byte数据，并且不发送应答信号
输入参数：无
输出参数：unsigned char：接收到的数据
版本：A1.0
**************************************************************************************/
unsigned char I2C_ReceiveByte_NoACK_1(void)
{
	unsigned char i,i2c_data;
	//
 	I2C_SDA1_1;
 	I2C_SCL1_0; 
 	i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		I2C_SCL1_1;
  		I2C1_NOP; 
  		i2c_data<<=1;
			//
  		if(I2C_SDA1_STATE)	i2c_data|=0x01; 
  
  		I2C_SCL1_0;  
  		I2C1_NOP;         
 	}
	I2C_SendNACK_1();
 	return i2c_data;
}


/*************************************************************************************
函数原型：unsigned char I2C_ReceiveByte_WithACK(void)
函数功能：模拟I2C接收1byte数据，并且发送应答信号
输入参数：无
输出参数：unsigned char：接收到的数据
版本：A1.0
**************************************************************************************/
unsigned char I2C_ReceiveByte_WithACK_1(void)
{
	unsigned char i,i2c_data;
	//
 	I2C_SDA1_1;
 	I2C_SCL1_0; 
 	i2c_data=0;
	//
 	for(i=0;i<8;i++)
 	{
  		I2C_SCL1_1;
  		I2C1_NOP; 
  		i2c_data<<=1;
			//
  		if(I2C_SDA1_STATE)	i2c_data|=0x01; 
  
  		I2C_SCL1_0;  
  		I2C1_NOP;         
 	}
	I2C_SendACK_1();
 	return i2c_data;
}


//-------------------------------------------------应用层代码------------------------------------------------//
/*************************************************************************************
函数原型：void I2C_Write_Single_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t reg_value)
函数功能：往从设备的单个寄存器中写入数据
输入参数：uint8_t device_addr：从设备地址
		  uint8_t reg_addr：   从设备中寄存器地址
		  uint8_t reg_value    要写入寄存器的数据		   
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_Write_Single_Reg_1(uint8_t device_addr,uint8_t reg_addr,uint8_t reg_value)
{
	I2C_START_1();
	I2C_SendByte_1(device_addr);				//圆点博士:发送陀螺仪写地址
	I2C_SendByte_1(reg_addr);  //圆点博士:发送陀螺仪PWM地址
	I2C_SendByte_1(reg_value); //圆点博士:发送陀螺仪PWM值
	I2C_STOP_1();	
}


/*************************************************************************************
函数原型：void I2C_Write_Multi_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t *value_buf_addr, uint8_t reg_number)
函数功能：往从设备的连续多个寄存器中写入数据，从设备会自动递增寄存器地址
输入参数：uint8_t device_addr：		从设备地址
		  uint8_t reg_addr：   		从设备中寄存器地址
		  uint8_t *value_buf_addr	要写入寄存器的数据组成的数组
		  uint8_t reg_number    		要写入的寄存器的个数		   
输出参数：无
版本：A1.0
**************************************************************************************/
void I2C_Write_Multi_Reg_1(uint8_t device_addr,uint8_t reg_addr,uint8_t *value_buf_addr, uint8_t reg_number)
{
	uint8_t i = 0;
	I2C_START_1();
	I2C_SendByte_1(device_addr);				//圆点博士:发送陀螺仪写地址
	I2C_SendByte_1(reg_addr);  //圆点博士:发送陀螺仪PWM地址
	for(i=0;i<reg_number;i++)
	{
		I2C_SendByte_1( (*(value_buf_addr++)) ); //圆点博士:发送陀螺仪PWM值
	}	
	I2C_STOP_1();	
}


/*************************************************************************************
函数原型：uint8_t I2C_Read_Single_Reg(uint8_t device_addr,uint8_t reg_addr) 
函数功能：读取从设备的单个寄存器值
输入参数：uint8_t device_addr：		从设备地址
		  uint8_t reg_addr：   		从设备中寄存器地址		   
输出参数：uint8_t：从设备寄存器值
版本：A1.0
**************************************************************************************/
uint8_t I2C_Read_Single_Reg_1(uint8_t device_addr,uint8_t reg_addr)   
{
	uint8_t reg_value;
	
	I2C_START_1();
	I2C_SendByte_1(device_addr);					//圆点博士:发送陀螺仪写地址
	I2C_SendByte_1(reg_addr);    				//圆点博士:发送陀螺仪寄存器地址
	I2C_START_1();
	I2C_SendByte_1(device_addr+1);      			//圆点博士:发送陀螺仪读地址
	reg_value=I2C_ReceiveByte_NoACK_1();			//圆点博士:读出陀螺仪寄存器值
	I2C_STOP_1();
	//
	return reg_value;
}


/*************************************************************************************
函数原型：void I2C_Read_Multi_Reg(uint8_t device_addr,uint8_t reg_addr, uint8_t *value_buf_addr, uint8_t reg_number) 
函数功能：读取从设备的连续多个寄存器值
输入参数：uint8_t device_addr：		从设备地址
		  uint8_t reg_addr：   		从设备中寄存器起始地址
		  uint8_t *value_buf_addr    用来存储寄存器值的数组首地址
		  uint8_t reg_number         要读取的寄存器的个数		   
输出参数：uint8_t：从设备寄存器值
版本：A1.0
**************************************************************************************/

void I2C_Read_Multi_Reg_1(uint8_t device_addr,uint8_t reg_addr, uint8_t *value_buf_addr, uint8_t reg_number)   
{
	uint8_t 	i = 0;
	uint8_t		uc_device_ack_data = 0xFF,	uc_reg_ack_data = 0xFF;    //test
	
	
//	do
//	{
		I2C_START_1();
		uc_device_ack_data = I2C_SendByte_1(device_addr);					//圆点博士:发送陀螺仪写地址
		uc_reg_ack_data = I2C_SendByte_1(reg_addr);    				//圆点博士:发送陀螺仪寄存器地址
//	}while( (!uc_device_ack_data)||(!uc_reg_ack_data) );   //test
	
	
//	do
//	{
		I2C_START_1();
		uc_reg_ack_data = I2C_SendByte_1(device_addr+1);      			//圆点博士:发送陀螺仪读地址
//	}while(!uc_reg_ack_data);
	
	
	for(i=0;i<reg_number-1;i++)
	{
		value_buf_addr[i] = I2C_ReceiveByte_WithACK_1();	
	}
	value_buf_addr[reg_number-1] = I2C_ReceiveByte_NoACK_1();
	I2C_STOP_1();
}



