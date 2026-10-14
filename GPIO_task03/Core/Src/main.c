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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

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
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
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
  /* USER CODE BEGIN 2 */

  __HAL_RCC_GPIOE_CLK_ENABLE();  // 使能 GPIOE的CLK时钟
  __HAL_RCC_GPIOB_CLK_ENABLE();

#define CRL_OFFSET  0x00   // 端口寄存器相对基地址的偏移量
#define IDR_OFFSET  0X08
#define ODR_OFFSET  0X0C
#define BSRR_OFFSET 0X10

  uint32_t * pBBase = (uint32_t *)0x40010C00;//定义指向GPIO基地址的指针
  uint32_t * pEBase = (uint32_t *)0x40011800;

  uint32_t * pBCrl = (uint32_t *)((uint32_t)pBBase + CRL_OFFSET);//定义指向寄存器地址的指针
  uint32_t * pBOdr = (uint32_t *)((uint32_t)pBBase + ODR_OFFSET);
  uint32_t * pBBsrr = (uint32_t *)((uint32_t)pBBase + BSRR_OFFSET);

  uint32_t * pECrl = (uint32_t *)((uint32_t)pEBase + CRL_OFFSET);
  uint32_t * pEIdr = (uint32_t *)((uint32_t)pEBase + IDR_OFFSET);
  uint32_t * pEOdr = (uint32_t *)((uint32_t)pEBase + ODR_OFFSET);
  uint32_t * pEBsrr = (uint32_t *)((uint32_t)pEBase + BSRR_OFFSET);

  *pECrl &= ~(1<<12 | 1<<13 | 1<<14);//设置PE3为上拉输入
  *pECrl |= (1<<15);
  *pEOdr |= (1<<3);

  *pBCrl &= ~(1<<16 | 1<<17 | 1<<18);//设置PE4为上拉输入
  *pBCrl |= (1<<19);
  *pBOdr |= (1<<4);

  int led1_state, led0_state;


  *pECrl &= ~(1<<21|1<<22|1<<23);//设置PE5为推挽输出
  *pECrl |= (1<<20);

  *pBCrl &= ~(1<<21 | 1<<22 | 1<<23);//设置PB5为推挽输出
  *pBCrl |= (1<<20);

  void LED0_setState(int led0_state)//定义函数，通过PE5的输出电平控制LED0的状态
  {
    if (led0_state == 0)
    {
      for (int i = 0; i < 10; i++)
      {
        *pEOdr &= ~(1<<5);
        for (volatile int j = 0; j < 72000; j++){};
        *pEOdr |= (1<<5);
        for (volatile int j = 0; j < 72000; j++){};
      }
    }
    else *pEOdr |= (1<<5);
  }

  void LED1_setState(int led1_state)//定义函数，通过PB5的输出电平控制LED1的状态
  {
    if (led1_state == 0)
    {
      *pBOdr |= (1<<5);
      for (int i = 0; i < 10; i++)
      {
        *pBOdr &= ~(1<<5);
        for (volatile int j = 0; j < 72000; j++){};
        *pBOdr |= (1<<5);
        for (volatile int j = 0; j < 72000; j++){};
      }
    }
    else *pBOdr |= (1<<5);
  }

  while (1)
  {
    led0_state = (*pEIdr >>3) & 0x01; //读取PE3、PE4的输入电平
    led1_state = (*pEIdr >>4) & 0x01;
    LED0_setState(led0_state);//调用函数
    LED1_setState(led1_state);
  }


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED0_Pin */
  GPIO_InitStruct.Pin = LED0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

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
