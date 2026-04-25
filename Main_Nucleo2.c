/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pitches.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TIM_FREQ 84000000
#define PWM_ARR 1400
#define SINE_SIZE 128
#define PI 3.1415926

uint32_t sine[SINE_SIZE];



uint8_t opcion = 0;
uint8_t rxData;
uint8_t flagRx = 0;

int nota_actual = 0;
uint32_t tiempo_nota = 0;
uint8_t reproduciendo = 0;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
float voltaje = 1.3;
uint32_t var;
uint32_t DAC_OUT[] = {0, 1241, 2482, 3723};
uint8_t i = 0;
int tempo = 1000;
int MarioMelody[] = {NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
		  NOTE_G5, REST, NOTE_G4, REST,
		  NOTE_C5, NOTE_G4, REST, NOTE_E4,
		  NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
		  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
		  REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,
		  NOTE_C5, NOTE_G4, REST, NOTE_E4,
		  NOTE_A4, NOTE_B4, NOTE_AS4, NOTE_A4,
		  NOTE_G4, NOTE_E5, NOTE_G5, NOTE_A5, NOTE_F5, NOTE_G5,
		  REST, NOTE_E5,NOTE_C5, NOTE_D5, NOTE_B4,

		  REST, NOTE_G5, NOTE_FS5, NOTE_F5, NOTE_DS5, NOTE_E5,
		  REST, NOTE_GS4, NOTE_A4, NOTE_C4, REST, NOTE_A4, NOTE_C5, NOTE_D5,
		  REST, NOTE_DS5, REST, NOTE_D5,
		  NOTE_C5, REST,



};

int MarioDuration[] = {  8, 8, 8, 8, 8, 8, 8,
		  4, 4, 8, 4,
		  4, 8, 4, 4,
		  4, 4, 8, 4,
		  8, 8, 8, 4, 8, 8,
		  8, 4,8, 8, 4,
		  4, 8, 4, 4,
		  4, 4, 8, 4,
		  8, 8, 8, 4, 8, 8,
		  8, 4,8, 8, 4,


		  4, 8, 8, 8, 4, 8,
		  8, 8, 8, 8, 8, 8, 8, 8,
		  4, 4, 8, 4,
		  2, 2,



};

int TetrisMelody[] = {  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
		  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
		  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
		  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,

		  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
		  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
		  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
		  NOTE_C5, NOTE_A4, NOTE_A4, REST,

		  NOTE_E5, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4,
		  NOTE_A4, NOTE_A4, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
		  NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
		  NOTE_C5, NOTE_A4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_C5,

		  NOTE_D5, NOTE_F5, NOTE_A5, NOTE_G5, NOTE_F5,
		  NOTE_E5, NOTE_C5, NOTE_E5, NOTE_D5, NOTE_C5,
		  NOTE_B4, NOTE_B4, NOTE_C5, NOTE_D5, NOTE_E5,
		  NOTE_C5, NOTE_A4, NOTE_A4, REST,

		  NOTE_E5, NOTE_C5,
		  NOTE_D5, NOTE_B4,
		  NOTE_C5, NOTE_A4,
		  NOTE_GS4, NOTE_B4, REST,
		  NOTE_E5, NOTE_C5,
		  NOTE_D5, NOTE_B4,
		  NOTE_C5, NOTE_E5, NOTE_A5,
		  NOTE_GS5};

int TetrisDuration[] = {  4, 8, 8, 4, 8, 8,
		  4, 8, 8, 4, 8, 8,
		  4, 8, 4, 4,
		  4, 4, 8, 4, 8, 8,

		  4, 8, 4, 8, 8,
		  4, 8, 4, 8, 8,
		  4, 8, 8, 4, 4,
		  4, 4, 4, 4,

		  4, 8, 8, 4, 8, 8,
		  4, 8, 8, 4, 8, 8,
		  4, 8, 4, 4,
		  4, 4, 8, 4, 8, 8,

		  4, 8, 4, 8, 8,
		  4, 8, 4, 8, 8,
		  4, 8, 8, 4, 4,
		  4, 4, 4, 4,

		  2, 2,
		  2, 2,
		  2, 2,
		  2, 4, 8,
		  2, 2,
		  2, 2,
		  4, 4, 2,
		  2};
void tocar_nota(int freq)
{
    if(freq == 0){
        HAL_TIM_Base_Stop(&htim6);
        return;
    }

    int sampleRate = SINE_SIZE * freq;
    int arr = (84000000 / sampleRate) - 1;

    TIM6->ARR = arr;
    HAL_TIM_Base_Start(&htim6);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_DAC_Init(void);
static void MX_TIM6_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */
void generarSeno(void);
int calcularARR(int freq);
void playTone1(int *tone, int *duration, int *pause, int Nsize);
void noTone1(void);

int presForFrecuency(int frecuency);
void playTone(int *tone, int *duration, int *pause, int size);
void noTone(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void generarSeno(void){
    for(int i=0;i<SINE_SIZE;i++){
        sine[i] = (uint32_t)((sin(2*PI*i/SINE_SIZE)+1)*2048);
    }
}

void playToneDAC(int *tone, int *duration, int size)
{
    int tempo = 1000;

    for(int i=0;i<size;i++)
    {
        int freq = tone[i];
        int dur = tempo / duration[i];

        if(freq == 0){
            HAL_Delay(dur);
            continue;
        }

        int sampleRate = SINE_SIZE * freq;
        int arr = (84000000 / sampleRate) - 1;

        TIM6->ARR = arr;

        HAL_Delay(dur);
    }
}





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
  MX_DMA_Init();
  MX_DAC_Init();
  MX_TIM6_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
  generarSeno();

  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)sine, SINE_SIZE, DAC_ALIGN_12B_R);
  HAL_TIM_Base_Start(&htim6);

  HAL_UART_Receive_IT(&huart3, &rxData, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_TIM_Base_Stop(&htim6);
  while (1)
  {
	    if(reproduciendo)
	    {
	        if(HAL_GetTick() - tiempo_nota >= (tempo / TetrisDuration[nota_actual]))
	        {
	            tiempo_nota = HAL_GetTick();

	            tocar_nota(TetrisMelody[nota_actual]);

	            nota_actual++;

	            if(nota_actual >= (sizeof(TetrisMelody)/sizeof(TetrisMelody[0])))
	            {
	                nota_actual = 0; // repetir
	            }
	        }
	    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief DAC Initialization Function
  * @param None
  * @retval None
  */
static void MX_DAC_Init(void)
{

  /* USER CODE BEGIN DAC_Init 0 */

  /* USER CODE END DAC_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC_Init 1 */

  /* USER CODE END DAC_Init 1 */

  /** DAC Initialization
  */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC_Init 2 */

  /* USER CODE END DAC_Init 2 */

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 1000-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

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
  huart2.Init.BaudRate = 115200;
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
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART3)
    {
        opcion = rxData;
        if(opcion == '1'){   // SONIDO APAGADO
            reproduciendo = 0;

            tocar_nota(0);  
            nota_actual = 0;
        }
        else if(opcion == '2'){ // SONIDO ENCENDIDO
            reproduciendo = 1;
            HAL_TIM_Base_Stop(&htim6); 
            nota_actual = 0;   
        }


        HAL_UART_Receive_IT(&huart3, &rxData, 1);
    }
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
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
