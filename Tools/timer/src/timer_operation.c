/*
 * timer_operation.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#include "timer_operation.h"

void TIMER1_Init(void)
{
	  TIM_HandleTypeDef handler;
	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};

	  handler.Instance = TIM1;
	  handler.Init.Prescaler = 36000;
	  handler.Init.CounterMode = TIM_COUNTERMODE_UP;
	  handler.Init.Period = 1000;
	  handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  handler.Init.RepetitionCounter = 0;
	  handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&handler) != HAL_OK){}
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&handler, &sClockSourceConfig) != HAL_OK){}
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&handler, &sMasterConfig) != HAL_OK){}

	  TIM1->DIER |= TIM_IT_UPDATE;
	  TIM1->CR1 |= TIM_CR1_CEN;

}

void TIMER2_Init(void)
{
  TIM_HandleTypeDef handler;
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  handler.Instance = TIM2;
  handler.Init.Prescaler = 36000;
  handler.Init.CounterMode = TIM_COUNTERMODE_UP;
  handler.Init.Period = 1000;
  handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&handler) != HAL_OK){}
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&handler, &sClockSourceConfig) != HAL_OK){}
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&handler, &sMasterConfig) != HAL_OK){}
}
