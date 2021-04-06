/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "global_variables.h"
#include "main_operation.h"
#include "stm32f1xx_hal_flash.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM1 update interrupt.
  */
void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_IRQn 0 */
	static float voltageAcc=0.0, currentAcc=0.0;
	static uint16_t ct=0;
//	static uint32_t batt_power;

	/* Runs ADC */
	ADC_Start(ADC1, 0, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_1CYCLE_5);
	batterySensorRaw.raw_battery_voltage = ADC_Read(ADC1);
	ADC_Start(ADC1, 1, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_1CYCLE_5);
	batterySensorRaw.raw_battery_current = ADC_Read(ADC1);
	ADC_Start(ADC1, 4, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_1CYCLE_5);
	batterySensorRaw.raw_battery_tempBat = ADC_Read(ADC1);
	ADC_Start(ADC1, 5, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_1CYCLE_5);
	batterySensorRaw.raw_battery_tempRel = ADC_Read(ADC1);

	/* Process ADC signal */
	MATH_MovingAverage(&batterySensorAvg, &batterySensorRaw);

//	voltageAcc = voltageAcc + (0.003192*batterySensorRaw.raw_battery_voltage + 0.4778);
//	currentAcc = currentAcc + (-0.0221*batterySensorRaw.raw_battery_current + 67.305);

	/* Convert Raw Integer to real voltage */
	voltageAcc = voltageAcc + (0.0045236*batterySensorRaw.raw_battery_voltage + 0.1228);
//	voltageAcc = voltageAcc + (0.003192*batterySensorRaw.raw_battery_voltage + 0.4778);

	/* Convert Raw Integer to real current by piece wise condition */
	if(batterySensorRaw.raw_battery_current < 1900)	{
		currentAcc = currentAcc + (-0.0347*batterySensorRaw.raw_battery_current + 68.405);
	}
	else {
		currentAcc = currentAcc + (-0.0302*batterySensorRaw.raw_battery_current + 58.72);
	}
//	if(batterySensorRaw.raw_battery_current < 1900)	{
//		currentAcc = currentAcc + (-0.0247*batterySensorRaw.raw_battery_current + 63.405);
//	}
//	else {
//		currentAcc = currentAcc + (-0.0202*batterySensorRaw.raw_battery_current + 53.72);
//	}


	if(ct == 511){ // pick 8 sine cycle
		batterySensor.mBATPT = voltageAcc / 512;
		batterySensor.mBATCT = currentAcc / 512;
		batterySensor.mPower = (batterySensor.mBATPT*batterySensor.mBATCT);
		voltageAcc = 0.0;
		currentAcc = 0.0;
		ct = 0;
	}
	else {ct++;}

	/* Check if data valid */
	if(batterySensorAvg.isValid)
	{
		/* Convert to True Voltage and Current */
//		batterySensor.mBATPT   = 0.003192*batterySensorAvg.avg_battery_voltage + 0.4778;
//		batterySensor.mBATCT   = -0.0221*batterySensorAvg.avg_battery_current + 67.305;
		batterySensor.mBATTemp = (uint8_t)(0.0288*batterySensorAvg.avg_battery_tempBat);
		batterySensor.mRELTemp = (uint8_t)(0.0288*batterySensorAvg.avg_battery_tempRel);

//		batterySensor.mPower   = (batterySensor.mBATPT*batterySensor.mBATCT);

		/* Put variable invalid */
		batterySensorAvg.isValid = false;
	}

	/* USER CODE END TIM1_UP_IRQn 0 */
	HAL_TIM_IRQHandler(&htim1);
	/* USER CODE BEGIN TIM1_UP_IRQn 1 */

  /* USER CODE END TIM1_UP_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	static uint8_t rIntCount	 = 0;
	static uint8_t battFullCount = 0;
	static char buffer[60];

	/* Find Internal resistance, it is done once at the beginning */
	if(stateFlag.rInt_flag)
	{
		if(rIntCount == 8)
		{
			vt = batterySensor.mBATPT;
			Rintern = (v0-vt) / batterySensor.mBATCT;
			rIntCount = 0;
			stateFlag.rInt_flag = false;
		}
		else {rIntCount++;}
	}

	/* Start Counting when the STATE exits STARTUP */
	if(!stateFlag.startUp_flag && !stateFlag.warning_flag)
	{
		/* Count SoC */
		SoH 	 = -0.0003*CycleCount + 1;
		capConst = SoH*capacity*36;
//		soct   = (float)((batterySensor.mBATCT)/(1512)); // 42 x 3600 = 151200, 100% -> 151200/100
		soct   = batterySensor.mBATCT/capConst;
		socSum = socSum + soct;
		socInt = (uint8_t)socSum;	// convert to unsigned integer

		/* Count Cycle and SoH */
		soct < 0.0 ? (socCycle = socCycle - soct):(socCycle = socCycle + soct);
		socCycleMem = (uint8_t)(socCycle);	// integer version of socCycle
		if(socCycleMem >= 200)
		{
			CycleCount++;
			socCycle = 0.0;
		}

		/******************/
		/* Update Display */
		/******************/
		/* SoC */
		sprintf(buffer,"%3u",socInt);
		ssdHandler.currentX	= 7;
		ssdHandler.currentY	= 12;
		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

		/* Voltage */
		sprintf(buffer,"%d.%02u", (int) batterySensor.mBATPT, (int) ((batterySensor.mBATPT - (int) batterySensor.mBATPT )*100)); // the method is somewhat tricky
		ssdHandler.currentX	= 64;
		ssdHandler.currentY	= 12;
		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

		/* Temperature */
		sprintf(buffer,"%3u",batterySensor.mBATTemp);
		ssdHandler.currentX	= 7;
		ssdHandler.currentY	= 45;
		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

//		sprintf(buffer,"%3u",soc0Int);
//		ssdHandler.currentX	= 35;
//		ssdHandler.currentY	= 45;
//		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

		/* Current */
//		sprintf(buffer,"%d.%02u", (int) batterySensor.mBATCT, (int) ((batterySensor.mBATCT - (int) batterySensor.mBATCT )*100)); // the method is somewhat tricky
//		ssdHandler.currentX	= 7;
//		ssdHandler.currentY	= 45;
//		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

		/* Power */
		sprintf(buffer,"%4ld ",batterySensor.mPower);
		ssdHandler.currentX	= 64;
		ssdHandler.currentY	= 45;
		SSD_PutString(buffer, &Font_11x18, &ssdHandler, SSD_COLOR_WHITE);

		SSD_UpdateScreen();
	}

	/* if error happens, red led is blinked */
	if(stateFlag.warning_flag)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6);
	}

	/* Check if battery already full */
	if((batterySensor.mBATPT >= 4.3) && ((batterySensor.mBATCT < 2.0) && (batterySensor.mBATCT >= 0.0)))
	{
		/* Count till 60 seconds to ensure condition is stable */
		if(battFullCount>60)
		{
			stateFlag.batFull_flag = true;
			battFullCount = 0;
		}
		else battFullCount++;
	}
	else
	{
		battFullCount = 0;
		stateFlag.batFull_flag =  false;
	}


//	sprintf(buffer, "Voltage : %lu \r\n", batterySensorAvg.avg_battery_voltage);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "Current : %lu \r\n", batterySensorAvg.avg_battery_current);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "Temp. 1 : %lu \r\n", batterySensorAvg.avg_battery_tempBat);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "Temp. 2  : %lu \r\n", batterySensorAvg.avg_battery_tempRel);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "V : %f \r\n", batterySensor.mBATPT);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "I : %f \r\n", batterySensor.mBATCT);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "rint : %f \r\n", rint);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	sprintf(buffer, "Power : %4ld \r\n", batterySensor.mPower);
//	HC05_Send(USART2, (uint8_t*)&buffer[0], strlen(buffer));
//	if(BMS_SendCommand(BASIC_INFO, (uint8_t*)&buffer[0]))
//	{
//		BMS_Send(USART1, buffer, 7);
//	}
//	HC05_Send(USART2, (uint8_t*)&buffer2[0], strlen(buffer2));
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	uint8_t data;
	static uint8_t 	count = 0;
	static uint8_t 	dataLength;
	static bool 	isBMSData = false;
	static uint8_t 	buffer[50];
	/* Read Data in receive buffer */
	if(BMS_DataIsAvailable(USART1))
	{
		data = BMS_Receive(USART1);
	}

	/* Received the valid data from BMS */
	if((isBMSData) && (count < dataLength+7))
	{
		buffer[count] = data;
		count++;
		if(count==4) // add one from "length location
		{
			/* Check if the response is correct */
			if((buffer[1]==0xA5) && (buffer[2]==0x00))
			{
				dataLength = buffer[3];
			}
			/* Restart if streams are incorrect */
			else
			{
				isBMSData = false;
				dataLength = 0;
				count = 0;
			}
		}
	}

	/* Check if Start Byte */
	if((data == 0xDD) && !(isBMSData))
	{
		isBMSData = true;
		buffer[count] = data;
		/* create a dummy data length */
		dataLength = 10;
		count++;
	}

	/* Check if checking finishes */
	if(count == dataLength+7)
	{
		/* Data Parse */
		isBMSData = false;
		count = 0;
		BMS_Send(USART1, buffer, dataLength+7);
	}

//	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	return;
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	uint8_t data;

	if(HC05_DataIsAvailable(USART2))
	{
		data = HC05_Receive(USART2);
		switch(data)
		{
			/* Charging LED */
			case '1' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // RED OFF
			  	  	   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // GREEN OFF
			  	  	   break;
			case '2' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // RED ON
	  	  	   	   	   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // GREEN OFF
					   break;
			case '3' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // RED OFF
 	   	   	   	   	   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET); // GREEN ON
					   break;

			/* Inverter LED */
			case '4' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // BLUE OFF
					   break;
			case '5' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); // BLUE ON
					   break;

			/* DC IN */
			case 'A' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // DC IN Relay Closed
					   break;
			case 'a' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // DC IN Relay Opened
					   break;

			/* DC USB */
			case 'B' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // DC OUT Relay Closed
					   break;
			case 'b' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // DC OUT Relay Opened
					   break;

			/* AC OUT */
			case 'C' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // AC OUT Relay Closed
					   break;
			case 'c' : HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // AC OUT Relay Opened
					   break;

			/* FAN */
			case 'D' : HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET); // Fan Relay Closed
					   break;
			case 'd' : HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); // Fan Relay Opened
					   break;

			/* Saving Flash */
			case 'G' : /* Save SoC record to FLASH */
//						HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
//					   buffWrPtr = buffWrPtr | (((uint32_t)(socInt) << 24) & (uint32_t)(0xFF000000));	// assign SoC
//					   buffWrPtr = buffWrPtr | (((uint32_t)(socCycleMem) << 16) & (uint32_t)(0x00FF0000));	// Fill soc counting information
//					   buffWrPtr = buffWrPtr | ((uint32_t)(CycleCount) 		 & (uint32_t)(0x0000FFFF));	// Fill cycle Count
//					   Flash_Write_Data(0x0801FBF4, &buffWrPtr, 1);
					   break;
		}
		HC05_Send(USART2, (uint8_t*)&data, 1);
	}
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
