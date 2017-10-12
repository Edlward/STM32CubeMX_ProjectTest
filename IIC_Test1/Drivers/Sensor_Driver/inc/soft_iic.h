#ifndef	_SOFT_IIC_H
#define _SOFT_IIC_H
#include "stm32f1xx_hal.h"

//typedef enum {I2C_READY = 0,I2C_BUS_BUSY,I2C_BUS_ERROR,I2C_NACK,I2C_ACK} I2C_Status;

void I2C_Configuration(void);
void I2C_Write_Single_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t reg_value);
void I2C_Write_Multi_Reg(uint8_t device_addr,uint8_t reg_addr,uint8_t *value_buf_addr, uint8_t reg_number);
uint8_t I2C_Read_Single_Reg(uint8_t device_addr,uint8_t reg_addr);
void I2C_Read_Multi_Reg(uint8_t device_addr,uint8_t reg_addr, uint8_t *value_buf_addr, uint8_t reg_number); 

#endif
