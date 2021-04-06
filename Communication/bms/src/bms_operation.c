/*
 * bms_operation.c
 *
 *  Created on: Oct 12, 2020
 *      Author: Lenovo
 */

#include "bms_operation.h"

void BMS_Init(void)
{
	  UART_HandleTypeDef handler;
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	  uint32_t tmpreg;
	  uint32_t pclk;

      /* Peripheral clock enable */
	  __HAL_RCC_USART1_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  /**USART1 GPIO Configuration
	  PA9     ------> USART1_TX
	  PA10     ------> USART1_RX
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  GPIO_InitStruct.Pin = GPIO_PIN_10;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	  /* UART Setting */
	  handler.Instance = USART1;
	  handler.Init.BaudRate = 9600;
	  handler.Init.WordLength = UART_WORDLENGTH_8B;
	  handler.Init.StopBits = UART_STOPBITS_1;
	  handler.Init.Parity = UART_PARITY_NONE;
	  handler.Init.Mode = UART_MODE_TX_RX;
	  handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  handler.Init.OverSampling = UART_OVERSAMPLING_16;

      /* USART1 interrupt Init */
	  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(USART1_IRQn);
	  /* Enable Interrupt */
	  __HAL_UART_ENABLE_IT(&handler, UART_IT_RXNE);

	  /* Ensure UART is disabled */
	  __HAL_UART_DISABLE(&handler);

	  /* Set the UART Communication parameters */
	  MODIFY_REG(handler.Instance->CR2, USART_CR2_STOP, handler.Init.StopBits);
	  tmpreg = (uint32_t)handler.Init.WordLength | handler.Init.Parity | handler.Init.Mode;
	  MODIFY_REG(handler.Instance->CR1,
	             (uint32_t)(USART_CR1_M | USART_CR1_PCE | USART_CR1_PS | USART_CR1_TE | USART_CR1_RE),
	             tmpreg);
	  MODIFY_REG(handler.Instance->CR3, (USART_CR3_RTSE | USART_CR3_CTSE), handler.Init.HwFlowCtl);
	  pclk = HAL_RCC_GetPCLK2Freq();
	  handler.Instance->BRR = UART_BRR_SAMPLING16(pclk, handler.Init.BaudRate);

	  /* In asynchronous mode, the following bits must be kept cleared:
	     - LINEN and CLKEN bits in the USART_CR2 register,
	     - SCEN, HDSEL and IREN  bits in the USART_CR3 register.*/
	  CLEAR_BIT(handler.Instance->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	  CLEAR_BIT(handler.Instance->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	  /* Enable the peripheral */
	  __HAL_UART_ENABLE(&handler);
}

void BMS_Send(USART_TypeDef* USARTx, uint8_t* Data, uint16_t count)
{
	uint16_t i;
	for(i=0; i< count; i++)
	{
		/* Wait to be ready, buffer empty */
		do { while (!((USARTx)->SR & UART_FLAG_TXE)); } while (0);
		/* Send Data */
		USARTx->DR =(uint16_t)(Data[i]);
		/* Wait to be ready, buffer empty */
		do { while (!((USARTx)->SR & UART_FLAG_TXE)); } while (0);
	}
}

bool BMS_DataIsAvailable(USART_TypeDef* USARTx)
{
	bool status = false;
	if (((USARTx->SR & USART_SR_RXNE) != RESET) && ((USARTx->CR1 & USART_CR1_RXNEIE) != RESET))
	{
		status = true;
	}

	return status;
}

uint8_t BMS_Receive(USART_TypeDef* USARTx)
{
	uint8_t data;

	data = (USARTx->DR & (uint8_t)0x00FF);
	return data;
}

bool BMS_SendCommand(COMMAND_TYPE_t code, uint8_t* buffer)
{
	bool status;

	status = true;
	switch(code)
	{
		case BASIC_INFO : buffer[0] = 0xDD;
						  buffer[1] = 0xA5;
						  buffer[2] = 0x03;
						  buffer[3] = 0x00;
						  buffer[4] = 0xFF;
						  buffer[5] = 0xFD;
						  buffer[6] = 0x77;
						  break;

		case CELL_VOLT	: buffer[0] = 0xDD;
		  	  	  	  	  buffer[1] = 0xA5;
		  	  	  	  	  buffer[2] = 0x04;
		  	  	  	  	  buffer[3] = 0x00;
		  	  	  	  	  buffer[4] = 0xFF;
		  	  	  	  	  buffer[5] = 0xFC;
		  	  	  	  	  buffer[6] = 0x77;
		  				  break;

		default			: status = false;
					  	  break;
	}
	return status;
}
