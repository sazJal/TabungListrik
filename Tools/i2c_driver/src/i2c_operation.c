/*
 * i2c_operation.c
 *
 *  Created on: Oct 14, 2020
 *      Author: Lenovo
 */

#include "i2c_operation.h"

void I2C_Init(I2C_HandleTypeDef* handler)
{
  /* USER CODE END I2C2_Init 1 */
  handler->Instance = I2C2;
  handler->Init.ClockSpeed = 100000;
  handler->Init.DutyCycle = I2C_DUTYCYCLE_2;
  handler->Init.OwnAddress1 = 240;
  handler->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  handler->Init.OwnAddress2 = 0;
  handler->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  handler->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(handler) != HAL_OK)
  {
  }
}

void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t ctrlByte, uint8_t data)
{
	/* Start */
	I2C_Start(I2Cx, address);
	/* Write Control Byte */
	I2C_WriteData(I2Cx, ctrlByte);
	/* Write Data */
	I2C_WriteData(I2Cx, data);
	/* Stop */
	I2C_Stop(I2Cx);
}

void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t ctrlByte, uint8_t* data, uint8_t count)
{
	/* Start */
	I2C_Start(I2Cx, address);
	/* Write Control Byte */
	I2C_WriteData(I2Cx, ctrlByte);
	while(count--){
		/* Write Data */
		I2C_WriteData(I2Cx, *data++);
	}
	/* Stop */
	I2C_Stop(I2Cx);
}

void I2C_Start(I2C_TypeDef * I2Cx, uint8_t address)
{
	/* Generate I2C Start Pulse */
	I2Cx->CR1 |= I2C_CR1_START;

	/* Wait till I2C is busy */
	TM_I2C_Timeout = 20000;
	while(!(I2Cx->SR1 & I2C_SR1_SB)){
		if(--TM_I2C_Timeout == 0x00)
		{
			break;
		}
	}
	/* Enable ack */
	I2Cx->CR1 |= I2C_CR1_ACK;

	/* Send write bit address with zero bit endian */
	I2Cx->DR = address & ~I2C_OAR1_ADD0;
	TM_I2C_Timeout = 20000;
	while(!(I2Cx->SR1 & I2C_SR1_ADDR)){
		if(--TM_I2C_Timeout == 0x00)
		{
			break;
		}
	}
	/* Read status register to clear ADDR flag */
	I2Cx->SR2;
}

void I2C_Stop(I2C_TypeDef* I2Cx)
{
	TM_I2C_Timeout = 20000;
	while (((!(I2Cx->SR1 & I2C_SR1_TXE)) || (!(I2Cx->SR1 & I2C_SR1_BTF)))) {
		if (--TM_I2C_Timeout == 0x00) {
			break;
		}
	}
	/* Generate Stop */
	I2Cx->CR1 |= I2C_CR1_STOP;
}

void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data)
{
	/* Wait till I2C is not busy anymore */
	TM_I2C_Timeout = 20000;
	while(!(I2Cx->SR1 & I2C_SR1_TXE) && TM_I2C_Timeout){
		TM_I2C_Timeout--;
	}
	/* Send I2C data */
	I2Cx->DR = data;
}

