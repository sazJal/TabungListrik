/*
 * i2c_operation.h
 *
 *  Created on: Oct 14, 2020
 *      Author: Lenovo
 */

#ifndef I2C_DRIVER_INC_I2C_OPERATION_H_
#define I2C_DRIVER_INC_I2C_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal_i2c.h"

I2C_HandleTypeDef hi2c2;

uint32_t TM_I2C_Timeout;

void I2C_Init(I2C_HandleTypeDef* handler);
void I2C_Write(I2C_TypeDef* I2Cx, uint8_t address, uint8_t ctrlByte, uint8_t data);
void I2C_WriteMulti(I2C_TypeDef* I2Cx, uint8_t address, uint8_t ctrlByte, uint8_t* data, uint8_t count);
void I2C_Start(I2C_TypeDef * I2Cx, uint8_t address);
void I2C_Stop(I2C_TypeDef* I2Cx);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);

#endif /* I2C_DRIVER_INC_I2C_OPERATION_H_ */
