/*
 * bms_operation.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#ifndef BMS_INC_BMS_OPERATION_H_
#define BMS_INC_BMS_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

UART_HandleTypeDef huart1;

typedef struct BMS_DATA_RAW
{
	uint16_t voltage_bat_1;
	uint16_t voltage_bat_2;
	uint16_t voltage_bat_3;
	uint16_t voltage_bat_4;
	uint16_t temperature;
	uint16_t voltage_pack;
	uint16_t current;
	uint16_t remaining_capacity;
	uint16_t nominal_capacity;
	uint16_t cycles;
	uint8_t  soc;
} BMS_DATA_RAW_t;

typedef enum COMMAND_TYPE
{
	BASIC_INFO = (uint8_t) 0x03,
	CELL_VOLT  = (uint8_t) 0x04
} COMMAND_TYPE_t;

void BMS_Init(void);
void BMS_Send(USART_TypeDef* USARTx, uint8_t* Data, uint16_t count);
bool BMS_DataIsAvailable(USART_TypeDef* USARTx);
uint8_t BMS_Receive(USART_TypeDef* USARTx);
bool BMS_SendCommand(COMMAND_TYPE_t code, uint8_t* buffer);

#endif /* BMS_INC_BMS_OPERATION_H_ */
