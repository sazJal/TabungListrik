/*
 * hc05_operation.h
 *
 *  Created on: Sep 25, 2020
 *      Author: Lenovo
 */

#ifndef HC05_OPERATION_H_
#define HC05_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"
#include "stdbool.h"
#include "string.h"
#include "stdio.h"

UART_HandleTypeDef huart1;

void HC05_Init(void);
void HC05_Send(USART_TypeDef* USARTx, uint8_t* Data, uint16_t count);
bool HC05_DataIsAvailable(USART_TypeDef* USARTx);
uint8_t HC05_Receive(USART_TypeDef* USARTx);

#endif /* HC05_OPERATION_H_ */
