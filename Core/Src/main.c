/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "rtc.h"
#include "spi.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "soil_moisture_sensor.h"
#include "hw-038.h"
#include "pump.h"
#include "led.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
float soil_moisture;
float water_height;
uint8_t rx_data = 0;
uint8_t soil[50];
uint8_t water[50];
const uint8_t MOIST_Y = 2;
const uint8_t LEVEL_Y = 3;
const uint8_t ERROR_Y = 15;
static uint8_t lastState = 0;
static uint8_t stableSamples = UINT32_MAX;
static uint8_t errorState = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
enum EDGE_TYPE edgeDetect(const register ubyte pinState, const register uint samples) {
    if (pinState != lastState) {
        stableSamples = 1;
        lastState = pinState;
    } else if(stableSamples <= samples){
        stableSamples++;
        if (stableSamples == samples) {
            switch(pinState) {
                case 0: return FALL;
                case 1: return RISE;
                default: return NONE;
            }
        }
    }
    return NONE;
}
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
//	__HAL_RCC_SYSCFG_CLK_ENABLE();
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
  MX_ADC1_Init();
  MX_SPI1_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
  const uint16_t WHITE = decodeRgbValue(255, 255, 255);
  const uint16_t BLACK = decodeRgbValue(0, 0, 0);
  const uint16_t GREEN = decodeRgbValue(15, 250, 15);
  const uint16_t BLUE = decodeRgbValue(65, 50, 250);
  const uint16_t RED = decodeRgbValue(250, 0, 0);

  // Init LCD display
  lcdInitialise(LCD_ORIENTATION3);
//  char *buf = "Test of the lcd display and this should be a very long text...";
//  uint16_t Xoffset = lcdTextX(strlen(buf) + 1) + 2;
//  uint16_t Yoffset = lcdTextY(2);
//  lcdPutSWithCursor(buf, lcdTextX(1), lcdTextY(3), WHITE, BLACK, 10);

  // Init sensors
  SoilMoistureSensor_Init(GPIOA, GPIO_PIN_0, &hadc1, ADC_CHANNEL_2);
  Hw_Init(GPIOA, GPIO_PIN_12, &hadc1, ADC_CHANNEL_4);

  Pump_Init();

  ledInit(GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_11);
  ledOn();

  if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET) {
	  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB); // clear the flag
	  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc); // Deactive the RTC wakeup

	  lcdPutSWithCursor("System woken up.", lcdTextX(1), lcdTextY(0), WHITE, BLACK, 5);
  }
  else {
	  lcdPutSWithCursor("System initialized.", lcdTextX(1), lcdTextY(0), WHITE, BLACK, 1);
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
	  soil_moisture = SoilMoistureSensor_GetCalibratedResult();
	  water_height = Hw_GetCalibratedResult();


	  lcdPutSWithCursor(soil, lcdTextX(1), lcdTextY(MOIST_Y), BLACK, BLACK, 0);
	  lcdPutSWithCursor(water, lcdTextX(1), lcdTextY(LEVEL_Y), BLACK, BLACK, 0);

	  LL_mDelay(100);

	  memset(soil, 0, strlen(soil));
	  memset(water, 0, strlen(water));

	  sprintf(soil, "MOIST: %.2f%%", soil_moisture);
	  sprintf(water, "LEVEL: %.2f", water_height);

	  lcdPutSWithCursor(soil, lcdTextX(1), lcdTextY(MOIST_Y), BLUE, BLACK, 0);
	  lcdPutSWithCursor(water, lcdTextX(1), lcdTextY(LEVEL_Y), BLUE, BLACK, 0);

	  if (errorState){
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, SET);
		  lcdPutSWithCursor("Error: No water!", lcdTextX(1), lcdTextY(ERROR_Y), RED, BLACK, 0);
		  break;
	  }
	  else {
		  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, RESET);
		  lcdPutSWithCursor("Status: Running!", lcdTextX(1), lcdTextY(ERROR_Y), GREEN, BLACK, 0);
	  }

//	  ledToggle();

	  LL_mDelay(2000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
	__HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_RESET); // Clear EXTI line if applicable
	HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	HAL_NVIC_ClearPendingIRQ(RCC_IRQn);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN1);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN2);
	// RTC_WAKEUPCLOCK_RTCCLK_DIV = 16
	// WTB = 16 / (40 kHz) = 0.0004; // 0.000002
	// WUC = ~5/WTB = 12500 = 0x30D4 // 2500000 = 0x2625A0
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 0x3A98, 16) != HAL_OK)
	{
		Error_Handler();
	}

  HAL_PWR_EnterSTANDBYMode();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
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

//  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//  {
//    Error_Handler();
//  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if (GPIO_Pin == ErrorReset_Pin && edgeDetect(HAL_GPIO_ReadPin(ErrorReset_GPIO_Port, ErrorReset_Pin), 20) != RISE)
	{
		errorState = !errorState;

		stableSamples = 1;
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

      while (1) {
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
