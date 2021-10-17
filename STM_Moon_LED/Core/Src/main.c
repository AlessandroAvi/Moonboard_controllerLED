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
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "keypad.h"
#include "LED.h"

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
bool BLUE_BUTTON = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

// *******************************************************************
// 					TUTORIAL CODE
// *******************************************************************






// When DMA finishes a send triggers a flag and stops the data transfer
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){

	if(htim == &htim2){
		HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
	}

	if(htim == &htim3){
		HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
	}

	datasentflag = 1;
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
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */

  // *********************************************
  // 			LCD initialization
  // *********************************************
  if(lcd16x2_i2c_init(&hi2c1)){
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }
  lcd16x2_i2c_clear();
  lcd16x2_i2c_2ndLine();
  lcd16x2_i2c_clear();
  lcd16x2_i2c_printf("      RESET       ");

  HAL_Delay(1000);

  // Enable HC 05 module
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

  // enable interrupt for the UART
  __HAL_UART_ENABLE_IT(&huart6, UART_IT_RXNE);

  lcd16x2_i2c_clear();
  lcd16x2_i2c_2ndLine();
  lcd16x2_i2c_clear();

  lcd16x2_i2c_printf("><>   FISH   <><");
  lcd16x2_i2c_2ndLine();
  lcd16x2_i2c_printf("===   BOARD  ===");


  // *********************************************
  // 			Data definition
  // *********************************************
  Problem p;			    // struct that contains the info related to the problem
  uint32_t problemID = 0;	// ID that reppresents the number of the problem

  passProlemPtr(&p);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  if(BLUE_BUTTON){
		  lcd16x2_i2c_clear();	// clear the LCD display

		  // Listen for the key pressed on the keypad
		  problemID = keypad_getNumber();

		  if(problemID==10000){
			  uint8_t mode=0;

			  while(1){

				  if(mode==0){
					  lcd16x2_i2c_clear();
					  lcd16x2_i2c_printf("DISCO MODE");
					  lcd16x2_i2c_2ndLine();
					  lcd16x2_i2c_printf("    /(^-^)/ ");
					  mode +=1;
				  }else if(mode==2){
					  lcd16x2_i2c_clear();
					  lcd16x2_i2c_printf("DISCO MODE");
					  lcd16x2_i2c_2ndLine();
					  lcd16x2_i2c_printf("    -(^-^)-  ");
					  mode +=1;
				  }else{
					  mode +=1;
					  if(mode==4) mode=0;
				  }
				  LED_randSetRand();

				  // Sends to LED strip signal
				  WS2811_Send();

				  HAL_Delay(250);
			  }


		  }else{


			  // Search and update the struct depending on the ID obtained
			  problem_fetch(&p,problemID);
			  // Generate a matrix containing colors for each LED according to problem
			  problem_genArray(&p);

			  // Display on led boulder info
			  lcd16x2_i2c_clear();
			  lcd16x2_i2c_printf("Name:");
			  lcd16x2_i2c_printf(p.name);
			  lcd16x2_i2c_2ndLine();
			  lcd16x2_i2c_printf("Grad:");
			  lcd16x2_i2c_printf(p.grade);

			  // Sends to LED strip signal
			  WS2811_Send();

			  // Reset button state
			  BLUE_BUTTON = false;
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	// interrupt blue button
	if(GPIO_Pin==B1_Pin){
		BLUE_BUTTON = true;
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
