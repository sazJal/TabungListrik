/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "global_variables.h"
#include "main_operation.h"
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

bool isValid = false;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  OPERATION_STATE_t state;
  ERROR_SIGN_t errorSign;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */

  /* USER CODE BEGIN 2 */
  MX_GPIO_Init();
  ADC_Init(&hadc1);
  HC05_Init();
  BMS_Init();

  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  HAL_TIM_Base_Start_IT(&htim1);
  HAL_TIM_Base_Start_IT(&htim2);

  /* Set Initial Display Format */
  SSD_Startup();
  SSD_Fill(SSD_COLOR_BLACK);
  SSD_DrawBitmap(0, 0, TalisLogo, 128, 64, SSD_COLOR_WHITE);
  SSD_UpdateScreen();

  /* Set Initial State */
  state = STARTUP;
  buttonFlag.bat_flag = false;
  buttonFlag.inv_flag = false;
  stateFlag.rInt_flag = false;
  stateFlag.startUp_flag = true;
  stateFlag.warning_flag = false;
  errorSign = SIGN_NORMAL;
  batterySensorAvg.isValid = false;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // BLUE
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // RED
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // GREEN
  while (1)
  {
	  /* Checking Button Status */
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)== GPIO_PIN_RESET)
	  {
		  while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)== GPIO_PIN_RESET){}
		  buttonFlag.bat_flag = true;
	  }
	  if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)== GPIO_PIN_RESET)
	  {
		  while(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)== GPIO_PIN_RESET){}
		  buttonFlag.inv_flag = true;
	  }

	  switch(state)
	  {
	  case STARTUP  : /* Access all informations */
		  	  	  	  HAL_Delay(2000);
		  	  	  	  v0 = batterySensor.mBATPT;
		  	  	  	  /* Connects USB and DC-DC as load */
				      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // DC OUT Relay Closed
				      stateFlag.rInt_flag = true;
					  while(stateFlag.rInt_flag){}
				      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // DC OUT Relay Opened

					  /* piece-wise OCV-SoC transformation */
					  ocv = batterySensor.mBATPT + batterySensor.mBATCT*Rintern;;
					  if 	  ((ocv > 11.2)  && (ocv <= 12.0)){soc0 = 2.5*ocv - 28;}
					  else if ((ocv > 12.0)  && (ocv <= 12.8)){soc0 = 9.0486*ocv - 106.48;}
					  else if ((ocv > 12.8)  && (ocv <= 13.0)){soc0 = 50*ocv - 630;}
					  else if ((ocv > 13.0)  && (ocv <= 13.168)){soc0 = 120.25*ocv - 1544.7;}
					  else if ((ocv > 13.168) && (ocv <= 13.44)){soc0 = 241.93*ocv - 3156.2;}
					  else if ((ocv > 13.44) && (ocv <= 13.76)){soc0 = 12.5*ocv - 72;}
					  else if (ocv  > 13.76) {soc0 = 100.0;}
					  else if (ocv <= 11.2)  {soc0 = 0.0;}
					  socInt = (uint8_t)(soc0);

					  soc0Int = socInt; // testing variable

					  /* Validate SoC info from Voc and Coulomb Counting (stored in flash) method */
					  socSum = soc0;	// initially, we presume that OCV method more correct

					  if(Flash_Read_Data(0x0801FBF4, &buffRdPtr))		// Retrieve any Flash content if any
					  {
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); 	// BLUE ON
						  socFlash 		= (buffRdPtr >> 24);			// obtaining saved SoC
						  socCycleMem 	= (buffRdPtr >> 16);	// obtaining saved SoC counting for Cycle Determination
						  socCycle		= socCycleMem*1.0;			// convert to float
						  CycleCount 	= buffRdPtr & 0xFFFF; 		// obtaining charge/discharge cycle
						  socSum 		= socFlash*1.0;

//						  deviation = socInt - socFlash; // check significant difference between save coulomb counting and OCV method
//						  if((deviation > -20) && (deviation < 20)){socSum = socFlash*1.0;} // if both aren't deviating significantly, choose Coulomb Method
					  }

					  /* Restarts Display */
					  SSD_Fill(SSD_COLOR_BLACK);
					  /* Put static labels of SoC, voltage, temperature, voltage, and Power */
					  ssdHandler.currentX	= 0;
					  ssdHandler.currentY	= 0;
					  SSD_PutString(SOC_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 65;
					  ssdHandler.currentY	= 0;
					  SSD_PutString(VOLT_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 0;
					  ssdHandler.currentY	= 33;
					  SSD_PutString(TEMP_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 65;
					  ssdHandler.currentY	= 33;
					  SSD_PutString(POWER_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  SSD_UpdateScreen();
					  stateFlag.startUp_flag = false;
		  	  	  	  state = IDLE;
		  	  	  	  break;

	  case IDLE		: state = IDLE;
		  	  	  	  /* Checking Transition Status */
	  	  	  	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); 	// BLUE OFF

					  // if warn flag active -> Redraw The Image
					  if(buttonFlag.bat_flag && !buttonFlag.inv_flag){
						  /* Connecting DC Charger */
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);	// DC IN Relay Closed
						  HAL_Delay(500); // wait sometimes letting the current flow from charger to battery
						  state = CHG;
						  }
					  if(!buttonFlag.bat_flag && buttonFlag.inv_flag){
						  /* Connecting AC Load & DC Converter*/
					      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // DC OUT Relay Closed
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // AC OUT Relay Closed
						  state = DSG;
						  }

		  	  	  	  break;

	  case CHG		: state = CHG;
					  /* LED status when SoC less than 95 % */
					  if(socInt < 95)
					  {
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // RED ON
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);	// GREEN OFF
					  }

					  /* Checking Transition Status */
	  	  	  	   	  if(buttonFlag.bat_flag && !buttonFlag.inv_flag)
					  {
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);// DC IN Relay Opened

						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); 	// RED OFF
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); 	// GREEN OFF


						  /* Stop the timer interrupt */
						  HAL_TIM_Base_Stop(&htim1);
						  HAL_TIM_Base_Stop(&htim2);
						  HAL_Delay(500);
						  /* Store the value to writing buffer */
					      buffWrPtr = buffWrPtr | (((uint32_t)(socInt) << 24) & (uint32_t)(0xFF000000));	// assign SoC
					      buffWrPtr = buffWrPtr | (((uint32_t)(socCycleMem) << 16) & (uint32_t)(0x00FF0000));	// Fill soc counting information
					      buffWrPtr = buffWrPtr | ((uint32_t)(CycleCount) 		 & (uint32_t)(0x0000FFFF));	// Fill cycle Count
						  /* Write the buffer to flash */
					      Flash_Write_Data(0x0801FBF4, &buffWrPtr, 1);
						  HAL_Delay(500);
						  /* Restart the timer interrupt */
						  HAL_TIM_Base_Start_IT(&htim1);
						  HAL_TIM_Base_Start_IT(&htim2);
						  /* Move to Idle state */

						  state = IDLE;
					  }

	  	  	  	   	  /* Force State */
	  	  	  	   	  if(stateFlag.batFull_flag) // condition is triggered in interrupt
	  	  	  	   	  {
	  	  	  	   		  if(socInt > 90)
	  	  	  	   		  {
		  	  	  	   		  socSum = 100.0;
	  	  	  	   		  }
	  	  	  	   		  else
	  	  	  	   		  {
	  	  	  	   			  state = WARNING; errorSign = SIGN_NP; stateFlag.warning_flag = true;
	  	  	  	   		  }
	  	  	  	   		  stateFlag.batFull_flag = false;
	  	  	  	   	  }

	  	  	  	  	  /* Protection Mechanism */
	  	  	  	   	  // charger not plugged or charger is reversed
	  	  	  	   	  if(batterySensor.mBATCT < -1.0)
	  	  	  	   	  {state = WARNING; errorSign = SIGN_NP; stateFlag.warning_flag = true;}
	  	  	  	  	  // Over Charge
					  if(socInt >= 100)
					  {state = WARNING; errorSign = SIGN_OCHG; stateFlag.warning_flag = true;}
	  	  	  	  	  // Over Current
					  if(batterySensor.mBATCT > 20.0)
					  {state = WARNING; errorSign = SIGN_OC; stateFlag.warning_flag = true;}
	  	  	  	  	  // Over Voltage
					  if(batterySensor.mBATPT > 15.0)
					  {state = WARNING; errorSign = SIGN_OV; stateFlag.warning_flag = true;}
					  /* Over Temperature */
					  if(batterySensor.mBATTemp > 50)
					  {state = WARNING; errorSign = SIGN_OT; stateFlag.warning_flag = true;}
	  	  	  	   	  break;

	  case DSG		: state = DSG;
	  	  	  	  	  /* Checking Transition Status */
				   	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); 	// BLUE ON

				   	  if (!buttonFlag.bat_flag && buttonFlag.inv_flag){
						  /* Disconnect AC OUT Relay */
				   		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);	// AC OUT Relay Opened
						  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); 	// DC OUT Relay Opened

						  /* Stop the timer interrupt */
						  HAL_TIM_Base_Stop(&htim1);
						  HAL_TIM_Base_Stop(&htim2);
						  HAL_Delay(500);
						  /* Store the value to writing buffer */
					      buffWrPtr = buffWrPtr | (((uint32_t)(socInt) << 24) & (uint32_t)(0xFF000000));	// assign SoC
					      buffWrPtr = buffWrPtr | (((uint32_t)(socCycleMem) << 16) & (uint32_t)(0x00FF0000));	// Fill soc counting information
					      buffWrPtr = buffWrPtr | ((uint32_t)(CycleCount) 		 & (uint32_t)(0x0000FFFF));	// Fill cycle Count
						  /* Write the buffer to flash */
					      Flash_Write_Data(0x0801FBF4, &buffWrPtr, 1);
						  HAL_Delay(500);
						  /* Restart the timer interrupt */
						  HAL_TIM_Base_Start_IT(&htim1);
						  HAL_TIM_Base_Start_IT(&htim2);
						  /* Move to Idle state */
						  state = IDLE;
					  }

				   	  /* Protection Mechanism */
	  	  	  	  	  // Over Current
					  if(batterySensor.mBATCT < -40.0)
					  {state = WARNING; errorSign = SIGN_OC; stateFlag.warning_flag = true;}
				   	  // Over Discharge
					  if(socInt < 10)
					  {state = WARNING; errorSign = SIGN_ODSG; stateFlag.warning_flag = true;}
					  /* Over Temperature */
					  if(batterySensor.mBATTemp > 50)
					  {state = WARNING; errorSign = SIGN_OT; stateFlag.warning_flag = true;}
		  	  	  	  break;

	  case WARNING	: /* Open All Relay */
		  	  	  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET); // DC OUT Relay Opened
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET); // DC IN Relay Opened
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET); // AC OUT Relay Opened

					  /* Change LED color to yellow */
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); 	// BLUE OFF

					  /* Update the warning display */
	  	  	  	  	  SSD_Fill(SSD_COLOR_BLACK);
		  	  	  	  switch(errorSign)
	  	  	  	  	  {
						  case SIGN_NORMAL 	: break;
						  case SIGN_OC		: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
							  	  	  	  	  SSD_PutString("Current", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
							  	  	  	  	  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
							  	  	  	  	  SSD_PutString("flowed", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
							  	  	  	  	  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
							  	  	  	  	  SSD_PutString("too high !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
							  	  	  	  	  SSD_DrawBitmap(0, 0, overcurrent, 128, 64, SSD_COLOR_WHITE);
							  	  	  	  	  break;
						  case SIGN_OV		: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
											  SSD_PutString("Input", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
											  SSD_PutString("voltage is", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
											  SSD_PutString("too High !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  SSD_DrawBitmap(0, 0, overvoltage, 128, 64, SSD_COLOR_WHITE);
							  	  	  	  	  break;
						  case SIGN_OCHG	: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
											  SSD_PutString("Battery", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
											  SSD_PutString("is", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
											  SSD_PutString("Full !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  SSD_DrawBitmap(0, 0, overcharge, 128, 64, SSD_COLOR_WHITE);
							  	  	  	  	  break;
						  case SIGN_ODSG	: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
											  SSD_PutString("Battery", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
											  SSD_PutString("is too", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
											  SSD_PutString("Low !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  SSD_DrawBitmap(0, 0, overdischarge, 128, 64, SSD_COLOR_WHITE);
							  	  	  	  	  break;
						  case SIGN_OT		: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
											  SSD_PutString("Battery", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
											  SSD_PutString("is over", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
											  SSD_PutString("heated !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  SSD_DrawBitmap(0, 0, overtemperature, 128, 64, SSD_COLOR_WHITE);
							  	  	  	  	  break;
						  case SIGN_NP		: ssdHandler.currentX	= 10; ssdHandler.currentY	= 15;
											  SSD_PutString("Charger", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 28;
											  SSD_PutString("may have", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  ssdHandler.currentX	= 10; ssdHandler.currentY	= 41;
											  SSD_PutString("issues !", &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
											  SSD_DrawBitmap(0, 0, nopower, 128, 64, SSD_COLOR_WHITE);
						  					  break;
	  	  	  	  	  }
		  	  	  	  SSD_UpdateScreen();
		  	  	  	  HAL_Delay(5000);

		  	  	  	  /* Restart the label after Warning */
		  	  	  	  SSD_Fill(SSD_COLOR_BLACK);
					  ssdHandler.currentX	= 0;
					  ssdHandler.currentY	= 0;
					  SSD_PutString(SOC_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 65;
					  ssdHandler.currentY	= 0;
					  SSD_PutString(VOLT_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 0;
					  ssdHandler.currentY	= 33;
					  SSD_PutString(TEMP_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
					  ssdHandler.currentX	= 65;
					  ssdHandler.currentY	= 33;
					  SSD_PutString(POWER_LABEL, &Font_7x10, &ssdHandler, SSD_COLOR_WHITE);
		  	  	  	  SSD_UpdateScreen();

		  	  	  	  /* Refresh Condition */
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // RED OFF
					  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET); // GREEN OFF
		  	  	  	  state = IDLE;
		  	  	  	  errorSign = SIGN_NORMAL;
		  	  	  	  stateFlag.warning_flag = false;
		  	  	  	  break;

	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  /* LED Green stays on when SoC is above 98% */
	  (socInt >= 98)?(HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET)):HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	  /* Fan Control Temperature */
	  (batterySensor.mRELTemp >= 40)?(HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET)):(HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET));

	  /* Refresh Button Flag */
	  buttonFlag.bat_flag = false;
	  buttonFlag.inv_flag = false;

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config 
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Regular Channel 
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 450;	// 36.000.000 / 450 = 80.000 Hz
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 25-1;	// 80.000 / 3.2 kHz = 25
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 36000;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000-1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 PB14 PB15 
                           PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
