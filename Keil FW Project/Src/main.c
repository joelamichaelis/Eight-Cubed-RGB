/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
//#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_TLCS 12
#define UART_TIMEOUT 500 //ms
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void previous_lyr_off(uint16_t activeLyr);
uint16_t current_lyr_on(uint16_t activeLyr);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

bool UPDATE_FRAME = 0;
bool commandReceived = 0;
bool initialLyrToggle = 1;

//Indicates active cube layer, must be global so timer IRQ can access.
uint8_t activeLyr = 0;
uint8_t uartRxIndex = 0;

//Data Arrays & Pointers shese should probably not be global
// Data Arrays
uint16_t data16[16*NUM_TLCS];
uint8_t data8[24*NUM_TLCS];

// Data Pointers
uint16_t *data16Ptr = &data16[0];
uint8_t *data8Ptr = &data8[0];										

// pin mapping - this should never need to be reconfigured now that all the cabling as been eliminated :^)
// could make const uint8_ts but saving space in flash
uint8_t redMap[64] = {178,181,160,163,166,145,148,151,189,186,175,172,169,158,155,152,130,133,112,115,118,97,100,103,141,138,127,124,121,110,107,104,82,85,64,67,70,49,52,55,93,90,79,76,73,62,59,56,34,37,16,19,22,1,4,7,45,42,31,28,25,14,11,8};
uint8_t grnMap[64] = {177,180,183,162,165,144,147,150,190,187,184,173,170,159,156,153,129,132,135,114,117,96,99,102,142,139,136,125,122,111,108,105,81,84,87,66,69,48,51,54,94,91,88,77,74,63,60,57,33,36,39,18,21,0,3,6,46,43,40,29,26,15,12,9};
uint8_t bluMap[64] = {176,179,182,161,164,167,146,149,191,188,185,174,171,168,157,154,128,131,134,113,116,119,98,101,143,140,137,126,123,120,109,106,80,83,86,65,68,71,50,53,95,92,89,78,75,72,61,58,32,35,38,17,20,23,2,5,47,44,41,30,27,24,13,10};

uint8_t uartTxBuffer[DEF_STR_LEN];
uint8_t uartRxBuffer[RAW_CLI_DATA_STR_LEN];

Frame_TypeDef frame;
Frame_TypeDef *framePtr;

	
	
cliData_TypeDef cliData;
cliData_TypeDef *cliDataPtr = &cliData;
		
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	
	HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_2);	//
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_OC_Start(&htim1, TIM_CHANNEL_4);	//this was working with ch1 even though thats set as a gpio!
	HAL_SPI_Init(&hspi1);
	tlc_clear(data16Ptr);
	UPDATE_FRAME = 1;
	
	
	//have to manually initialize pointers 
	framePtr = &frame;
	frame.lyr0Ptr = &frame.lyr0;
	frame.lyr1Ptr = &frame.lyr1;
	frame.lyr2Ptr = &frame.lyr2;
	frame.lyr3Ptr = &frame.lyr3;
	frame.lyr4Ptr = &frame.lyr4;
	frame.lyr5Ptr = &frame.lyr5;
	frame.lyr6Ptr = &frame.lyr6;
	frame.lyr7Ptr = &frame.lyr7;
	frame.lyr0.redArrPtr = &frame.lyr0.redArray[0];
	frame.lyr0.grnArrPtr = &frame.lyr0.grnArray[0];
	frame.lyr0.bluArrPtr = &frame.lyr0.bluArray[0];
	frame.lyr1.redArrPtr = &frame.lyr1.redArray[0];
	frame.lyr1.grnArrPtr = &frame.lyr1.grnArray[0];
	frame.lyr1.bluArrPtr = &frame.lyr1.bluArray[0];
	frame.lyr2.redArrPtr = &frame.lyr2.redArray[0];
	frame.lyr2.grnArrPtr = &frame.lyr2.grnArray[0];
	frame.lyr2.bluArrPtr = &frame.lyr2.bluArray[0];
	frame.lyr3.redArrPtr = &frame.lyr3.redArray[0];
	frame.lyr3.grnArrPtr = &frame.lyr3.grnArray[0];
	frame.lyr3.bluArrPtr = &frame.lyr3.bluArray[0];
	frame.lyr4.redArrPtr = &frame.lyr4.redArray[0];
	frame.lyr4.grnArrPtr = &frame.lyr4.grnArray[0];
	frame.lyr4.bluArrPtr = &frame.lyr4.bluArray[0];
	frame.lyr5.redArrPtr = &frame.lyr5.redArray[0];
	frame.lyr5.grnArrPtr = &frame.lyr5.grnArray[0];
	frame.lyr5.bluArrPtr = &frame.lyr5.bluArray[0];
	frame.lyr6.redArrPtr = &frame.lyr6.redArray[0];
	frame.lyr6.grnArrPtr = &frame.lyr6.grnArray[0];
	frame.lyr6.bluArrPtr = &frame.lyr6.bluArray[0];
	frame.lyr7.redArrPtr = &frame.lyr7.redArray[0];
	frame.lyr7.grnArrPtr = &frame.lyr7.grnArray[0];
	frame.lyr7.bluArrPtr = &frame.lyr7.bluArray[0];
	
	HAL_UART_Receive_IT(&huart1, &cliData.rawTextInput[uartRxIndex], 1);
	//frame_set_single_led_color(framePtr,0,0x0FFF0FFF0FFF,45);
	for(uint8_t jk=0;jk<64;jk++)
	{
		lyr_frame_set_single_led_color(frame.lyr0Ptr, 0x0FFF0FFF0FFF, jk);
	}
	
	UPDATE_FRAME = true;
	HAL_Delay(1000);
	
	lcd20x4_i2c_init(&hi2c1);
	lcd20x4_i2c_clear();
	lcd20x4_i2c_1stLine();
	lcd20x4_i2c_printf("8^3 RGB Begin!");
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if(commandReceived==true)
		{
			commandReceived = false;
			cli_parse_data(cliDataPtr);
			cli_interpret_data(cliDataPtr,framePtr);
			cli_formulate_response(cliDataPtr);
			if(cliData.responseEnum != Null) HAL_UART_Transmit(&huart1, &cliData.responseStr[0], cliData.responseLen, UART_TIMEOUT);
			cli_data_cleanse(cliDataPtr); // resets everything except the rawTextInput
		}
		HAL_Delay(10);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 16383;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

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

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 1;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

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
  huart1.Init.BaudRate = 115200;
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, XLAT_3V3_Pin|BLANK_3V3_Pin|ESP8266_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, DCPRG_3V3_Pin|VPRG_3V3_Pin|L3_EN_Pin|L4_EN_Pin 
                          |L5_EN_Pin|L6_EN_Pin|L7_EN_Pin|L8_EN_Pin 
                          |L1_EN_Pin|L2_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : XLAT_3V3_Pin BLANK_3V3_Pin ESP8266_EN_Pin */
  GPIO_InitStruct.Pin = XLAT_3V3_Pin|BLANK_3V3_Pin|ESP8266_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : DCPRG_3V3_Pin VPRG_3V3_Pin L3_EN_Pin L4_EN_Pin 
                           L5_EN_Pin L6_EN_Pin L7_EN_Pin L8_EN_Pin 
                           L1_EN_Pin L2_EN_Pin */
  GPIO_InitStruct.Pin = DCPRG_3V3_Pin|VPRG_3V3_Pin|L3_EN_Pin|L4_EN_Pin 
                          |L5_EN_Pin|L6_EN_Pin|L7_EN_Pin|L8_EN_Pin 
                          |L1_EN_Pin|L2_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : XERR_Pin */
  GPIO_InitStruct.Pin = XERR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(XERR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Keypad_IRQ_Pin Wheel_IRQ_Pin */
  GPIO_InitStruct.Pin = Keypad_IRQ_Pin|Wheel_IRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	/*
	currentAction = Menu_Read_MPR121(hi2c1, GPIO_Pin, currentAction);
	if (currentAction == ACTION_HALT_ANIMATION) HALT_ANIMATION = true;
	*/
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	
	if(initialLyrToggle == true)	//necessary because algorithm wasn't designed to handle inital condition where all lyrs are off
	{	
		initialLyrToggle = false;
		previous_lyr_off(activeLyr);
	}
	
	if(UPDATE_FRAME==0)
	{
		previous_lyr_off(activeLyr);
		HAL_GPIO_WritePin(BLANK_3V3_GPIO_Port, BLANK_3V3_Pin, GPIO_PIN_SET);
		for(uint16_t dummyVar =1;dummyVar<15;dummyVar++); // short delay (approx 3us)
		HAL_GPIO_WritePin(BLANK_3V3_GPIO_Port, BLANK_3V3_Pin, GPIO_PIN_RESET);
		activeLyr =  current_lyr_on(activeLyr);
		return;
	}
	
	if(UPDATE_FRAME==1)
	{
		previous_lyr_off(activeLyr);
		lyr_frame_convert(frame.lyr0,data16Ptr); //previous usage
		//frame_convert(frame,activeLyr,data16Ptr);
		tlc_spi_convert(data16Ptr, data8Ptr);
		HAL_SPI_Transmit(&hspi1, data8Ptr, 24*NUM_TLCS, 10);
		HAL_GPIO_WritePin(BLANK_3V3_GPIO_Port, BLANK_3V3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(XLAT_3V3_GPIO_Port, XLAT_3V3_Pin, GPIO_PIN_SET);
		for(uint16_t dummyVar =1;dummyVar<15;dummyVar++); // short delay (approx 3us)
		HAL_GPIO_WritePin(XLAT_3V3_GPIO_Port, XLAT_3V3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(BLANK_3V3_GPIO_Port, BLANK_3V3_Pin, GPIO_PIN_RESET);
		UPDATE_FRAME=0;
		activeLyr =  current_lyr_on(activeLyr);
		return;
	}
}
	
void previous_lyr_off(uint16_t activeLyr)
{
	if(activeLyr == 0) HAL_GPIO_TogglePin(L8_EN_GPIO_Port, L8_EN_Pin);
	if(activeLyr == 1) HAL_GPIO_TogglePin(L1_EN_GPIO_Port, L1_EN_Pin);
	if(activeLyr == 2) HAL_GPIO_TogglePin(L2_EN_GPIO_Port, L2_EN_Pin);
	if(activeLyr == 3) HAL_GPIO_TogglePin(L3_EN_GPIO_Port, L3_EN_Pin);
	if(activeLyr == 4) HAL_GPIO_TogglePin(L4_EN_GPIO_Port, L4_EN_Pin);
	if(activeLyr == 5) HAL_GPIO_TogglePin(L5_EN_GPIO_Port, L5_EN_Pin);
	if(activeLyr == 6) HAL_GPIO_TogglePin(L6_EN_GPIO_Port, L6_EN_Pin);
	if(activeLyr == 7) HAL_GPIO_TogglePin(L7_EN_GPIO_Port, L7_EN_Pin);
}
	
uint16_t current_lyr_on(uint16_t activeLyr)
{
	if(activeLyr == 0) HAL_GPIO_TogglePin(L1_EN_GPIO_Port, L1_EN_Pin);
	if(activeLyr == 1) HAL_GPIO_TogglePin(L2_EN_GPIO_Port, L2_EN_Pin);
	if(activeLyr == 2) HAL_GPIO_TogglePin(L3_EN_GPIO_Port, L3_EN_Pin);
	if(activeLyr == 3) HAL_GPIO_TogglePin(L4_EN_GPIO_Port, L4_EN_Pin);
	if(activeLyr == 4) HAL_GPIO_TogglePin(L5_EN_GPIO_Port, L5_EN_Pin);
	if(activeLyr == 5) HAL_GPIO_TogglePin(L6_EN_GPIO_Port, L6_EN_Pin);
	if(activeLyr == 6) HAL_GPIO_TogglePin(L7_EN_GPIO_Port, L7_EN_Pin);
	if(activeLyr == 7) HAL_GPIO_TogglePin(L8_EN_GPIO_Port, L8_EN_Pin);
	activeLyr++;
	if(activeLyr==8) activeLyr = 0;
	return activeLyr;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(cliData.rawTextInput[uartRxIndex] == '\n') 
	{
		commandReceived = true;
		uartRxIndex = 0;
	}
	
	else uartRxIndex++;
	
	HAL_UART_Receive_IT(&huart1, &cliData.rawTextInput[uartRxIndex], 1);
}

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
