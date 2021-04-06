/*
 * analog_operation.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#include "analog_operation.h"

void ADC_Init(ADC_HandleTypeDef* handler)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  handler->Instance = ADC1;
  handler->Init.ScanConvMode = ADC_SCAN_DISABLE;
  handler->Init.ContinuousConvMode = DISABLE;
  handler->Init.DiscontinuousConvMode = ENABLE;
  handler->Init.NbrOfDiscConversion = 8;
  handler->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  handler->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  handler->Init.NbrOfConversion = 8;

  /* Initialization of IO for ADC */
  if (HAL_ADC_Init(handler) != HAL_OK){}
}

void ADC_Start(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
	uint32_t wait_loop_index = 0U;
	ADC_RegularChannelConfig(ADCx, ADC_Channel, Rank, ADC_SampleTime);
	ADCx->CR2 |= (uint32_t)ADC_CR2_ADON_Msk;
    wait_loop_index = (1UL * (SystemCoreClock / 1000000U));
    while(wait_loop_index != 0U)
    {
      wait_loop_index--;
    }
   	ADCx->CR2 |= (uint32_t)ADC_CR2_SWSTART_Msk;
}

uint16_t ADC_Read(ADC_TypeDef* ADCx)
{
	uint32_t timeout = 0xFFF;
	/* Wait till done */
	while (!(ADCx->SR & ADC_SR_EOC)) {
		if (timeout-- == 0x00) {
			return 0;
		}
	}

	/* Return result */
	return ADCx->DR;
}

void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
	uint32_t tmpreg1 = 0, tmpreg2 = 0;

	/* Get the old register value */
	tmpreg1 = ADCx->SMPR2;
	/* Calculate the mask to clear */
	tmpreg2 = ADC_SMPR2_SMP0_Msk << (3 * ADC_Channel);
	/* Clear the old channel sample time */
	tmpreg1 &= ~tmpreg2;
	/* Calculate the mask to set */
	tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
	/* Set the new channel sample time */
	tmpreg1 |= tmpreg2;
	/* Store the new register value */
	ADCx->SMPR2 = tmpreg1;

	/* For Rank 1 to 6 */
	if (Rank < 7)
	{
		 /* Get the old register value */
		 tmpreg1 = ADCx->SQR3;
		 /* Calculate the mask to clear */
		 tmpreg2 = ADC_SQR3_SQ1_Msk << (5 * (Rank - 1));
		 /* Clear the old SQx bits for the selected rank */
		 tmpreg1 &= ~tmpreg2;
		 /* Calculate the mask to set */
		 tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
		 /* Set the SQx bits for the selected rank */
		 tmpreg1 |= tmpreg2;
		 /* Store the new register value */
		 ADCx->SQR3 = tmpreg1;
	}
   /* For Rank 7 to 12 */
   else if (Rank < 13)
   {
		 /* Get the old register value */
		 tmpreg1 = ADCx->SQR2;
		 /* Calculate the mask to clear */
		 tmpreg2 = ADC_SQR2_SQ7_Msk << (5 * (Rank - 7));
		 /* Clear the old SQx bits for the selected rank */
		 tmpreg1 &= ~tmpreg2;
		 /* Calculate the mask to set */
		 tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
		 /* Set the SQx bits for the selected rank */
		 tmpreg1 |= tmpreg2;
		 /* Store the new register value */
		 ADCx->SQR2 = tmpreg1;
   }
   /* For Rank 13 to 16 */
   else
   {
		 /* Get the old register value */
		 tmpreg1 = ADCx->SQR1;
		 /* Calculate the mask to clear */
		 tmpreg2 = ADC_SQR1_SQ13_Msk << (5 * (Rank - 13));
		 /* Clear the old SQx bits for the selected rank */
		 tmpreg1 &= ~tmpreg2;
		 /* Calculate the mask to set */
		 tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
		 /* Set the SQx bits for the selected rank */
		 tmpreg1 |= tmpreg2;
		 /* Store the new register value */
		 ADCx->SQR1 = tmpreg1;
   }
}
