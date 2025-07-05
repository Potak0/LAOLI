/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "string.h"
#include "vfd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define BUF_SIZE 16 // Define the size of the receive buffer
// #define CMD_QUERY_PLAYMODE {0x04, 0xFB, 0x01, 0x00, 0x00} // Query play mode command
// #define CMD_SET_PLAY {0x04, 0xFB, 0x01, 0x01, 0x01}       // Play command for the audio player
// #define CMD_SET_PAUSE {0x04, 0xFB, 0x01, 0x02, 0x02}      // Pause command for the audio player

// #define CMD_SET_PREV {0x04, 0xFB, 0x01, 0x04, 0x04} // Previous track command for the audio player
// #define CMD_SET_NEXT {0x04, 0xFB, 0x01, 0x05, 0x05} // Next track command for the audio player

// #define CMD_SW_2_SDCARD {0x04, 0xFB, 0x01, 0x0B, 0x0B} // Switch to SD card command

// #define CMD_QUERY_CURRENT_TRACK {0x04, 0xFB, 0x01, 0x0E, 0x0E} // Query current track command
// #define CMD_QUERY_TRACK_NAME {0x04, 0xFB, 0x01, 0x15, 0x15}    // Query track name command

// #define CMD_QUERY_CURRENT_PLAYTIME_EN {0x05, 0xFA, 0x01, 0x02, 0x02} // Query current playtime command

// #define CMD_QUERY_VOLUME {0x06, 0xF9, 0x01, 0x00, 0x00}     // Query volume command
// #define CMD_SET_VOLUME(x) {0x06, 0xF9, 0x02, 0x01, x, 0x02+x} // Set volume command
// #define CMD_SET_VOLUME_UP {0x06, 0xF9, 0x01, 0x02, 0x02}    // Volume up command
// #define CMD_SET_VOLUME_DOWN {0x06, 0xF9, 0x01, 0x03, 0x03} // Volume down command

// #define CMD_SET_PLAYMODE_CIRCLE {0x0B, 0xF4, 0x02, 0x01, 0x00, 0x02} // Set play mode to circle command

// #define CMD_SET_SLEEP {0x0D,0xF3,0x01,0x01,0x02} // Set sleep command

const uint8_t CMD_QUERY_PLAYMODE[] = {0x04, 0xFB, 0x01, 0x00, 0x00}; // Query play mode command
const uint8_t CMD_SET_PLAY[] = {0x04, 0xFB, 0x01, 0x01, 0x01};       // Play command for the audio player
const uint8_t CMD_SET_PAUSE[] = {0x04, 0xFB, 0x01, 0x02, 0x02};      // Pause command for the audio player

const uint8_t CMD_SET_PREV[] = {0x04, 0xFB, 0x01, 0x04, 0x04}; // Previous track command for the audio player
const uint8_t CMD_SET_NEXT[] = {0x04, 0xFB, 0x01, 0x05, 0x05}; // Next track command for the audio player

const uint8_t CMD_SW_2_SDCARD[] = {0x04, 0xFB, 0x01, 0x0B, 0x0B}; // Switch to SD card command

const uint8_t CMD_QUERY_CURRENT_TRACK[] = {0x04, 0xFB, 0x01, 0x0E, 0x0E}; // Query current track command
const uint8_t CMD_QUERY_TRACK_NAME[] = {0x04, 0xFB, 0x01, 0x15, 0x15};    // Query track name command

const uint8_t CMD_QUERY_CURRENT_PLAYTIME_EN[] = {0x05, 0xFA, 0x01, 0x02, 0x02}; // Query current playtime command

const uint8_t CMD_QUERY_VOLUME[] = {0x06, 0xF9, 0x01, 0x00, 0x00};     // Query volume command
const uint8_t CMD_SET_VOLUME[] = {0x06, 0xF9, 0x02, 0x01, 0x00, 0x02}; // Set volume command (example: x = 0)
const uint8_t CMD_SET_VOLUME_UP[] = {0x06, 0xF9, 0x01, 0x02, 0x02};    // Volume up command
const uint8_t CMD_SET_VOLUME_DOWN[] = {0x06, 0xF9, 0x01, 0x03, 0x03};  // Volume down command

const uint8_t CMD_SET_PLAYMODE_CIRCLE[] = {0x0B, 0xF4, 0x02, 0x01, 0x00, 0x02}; // Set play mode to circle command

const uint8_t CMD_SET_SLEEP[] = {0x0D, 0xF3, 0x01, 0x01, 0x02}; // Set sleep command

struct
{
  uint8_t playmode,
      volume,
      current_track,
      current_playtime_en,
      current_playtim_min,
      current_playtim_sec,
      track_name[12];
} mp3_status = {
    .playmode = 0,            // Default play mode
    .volume = 10,             // Default volume
    .current_track = 0,       // Default current track
    .current_playtime_en = 1, // Default current playtime enable
    .current_playtim_min = 0, // Default current playtime minutes
    .current_playtim_sec = 0, // Default current playtime seconds
    .track_name = {0}         // Default track name array
};

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE BEGIN PV */
uint8_t rx_buffer[BUF_SIZE]; // 创建接收缓存,大小为BUF_SIZE
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// gpio interrupt
int dir = 0;
int ec11_cnt = 0; // Counter for encoder pulses
// void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
// {
//   if (GPIO_Pin == GPIO_PIN_1) // Check if the interrupt is from PA1
//   {
//     ec11_cnt+=HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ? 1 : -1; // Increment or decrement based on PA0 state
//   }
// }

void UART_Instruction_Transmit(UART_HandleTypeDef *huart, uint8_t *msg, uint16_t size)
{
  // Transmit the message over UART
  static int count = 0;
  static int query_cnt = 0;
  static uint8_t msg_buf[10] = {0};
  if (count++ == 200) // Transmit every 100th call
  {
    count = 0;                                     // Reset count
    if (strcmp((char *)msg, (char *)msg_buf) != 0) // Check if the message is different from the last transmitted message
    {
      memcpy(msg_buf, msg, size); // Store the current message
    }
    else
    {
      // if the message is the same,enter circular query mode
      switch (query_cnt++)
      {
      case 0:
        memcpy(msg_buf, CMD_QUERY_PLAYMODE, sizeof(CMD_QUERY_PLAYMODE));
        size = sizeof(CMD_QUERY_PLAYMODE);
        break;
      case 1:
        memcpy(msg_buf, CMD_QUERY_CURRENT_TRACK, sizeof(CMD_QUERY_CURRENT_TRACK));
        size = sizeof(CMD_QUERY_CURRENT_TRACK);
        break;
      case 2:
        memcpy(msg_buf, CMD_QUERY_TRACK_NAME, sizeof(CMD_QUERY_TRACK_NAME));
        size = sizeof(CMD_QUERY_TRACK_NAME);
        break;
      case 3:
        memcpy(msg_buf, CMD_QUERY_CURRENT_PLAYTIME_EN, sizeof(CMD_QUERY_CURRENT_PLAYTIME_EN));
        size = sizeof(CMD_QUERY_CURRENT_PLAYTIME_EN);
        break;
      case 4:
        memcpy(msg_buf, CMD_QUERY_VOLUME, sizeof(CMD_QUERY_VOLUME));
        size = sizeof(CMD_QUERY_VOLUME);
        query_cnt = 0; // Reset query count
        break;
      }
    }
    HAL_UART_Transmit(huart, msg, size, HAL_MAX_DELAY);
  }
  else
  {
    HAL_Delay(1);
  }
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART2)
  {
    // int cnt = BUF_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
    // HAL_UART_Transmit(&huart2, rx_buffer, cnt, 0xffff);	//将接受到的数据再发回上位机
    // memset(rx_buffer, 0, cnt);
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
  MX_TIM2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  // Start the encoder interface
  // VFD_init(&hspi1); // Initialize the VFD display

  // VFD_test_out(); // Test the VFD display
  EN_1;
  HAL_Delay(100);
  VFD_init();
  S1201_WriteStr(0, "ABCDEFGH");
  S1201_show();

  HAL_Delay(5000);
  uint8_t msg_sdcard[] = {0x04, 0xFB, 0x01, 0x0B, 0x0B};
  uint8_t msg_playmode[] = {0x0B, 0xF4, 0x02, 0x01, 0x00, 0x02};
  uint8_t msg_play[] = {0x04, 0xFB, 0x01, 0x01, 0x01};
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET); // init pwamp
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
  HAL_UART_Transmit(&huart2, msg_sdcard, sizeof(msg_sdcard), HAL_MAX_DELAY);
  HAL_Delay(200);
  HAL_UART_Transmit(&huart2, msg_playmode, sizeof(msg_playmode), HAL_MAX_DELAY);
  HAL_Delay(200);
  HAL_UART_Transmit(&huart2, msg_play, sizeof(msg_play), HAL_MAX_DELAY);
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_UARTEx_ReceiveToIdle_DMA(&huart2, rx_buffer, BUF_SIZE);
  while (1)
  {
    dir = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim2) ? -1 : 1; // Check counting direction
    ec11_cnt = 0.5 * (short)__HAL_TIM_GET_COUNTER(&htim2); // Get the current counter value

    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_6); // Toggle LED on PB6
    HAL_Delay(50);                         // Delay for 500 ms
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7); // Toggle LED on PB7
    HAL_Delay(50);                         // Delay for 500 ms

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_5_6_7_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_5_6_7_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_6_7_IRQn);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA5 PA6 PA7 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB3 PB4 PB5
                           PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
