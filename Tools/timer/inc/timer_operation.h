/*
 * timer_operation.h
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#ifndef TIMER_OPERATION_H_
#define TIMER_OPERATION_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_tim.h"
#include "stdbool.h"

void TIMER1_Init(void);
void TIMER2_Init(void);

#endif /* TIMER_OPERATION_H_ */
