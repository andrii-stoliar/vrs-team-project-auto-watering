/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 7;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  LL_SPI_DisableNSSPulseMgt(SPI1);
  /* USER CODE BEGIN SPI1_Init 2 */
  SPI1->CR2 |= 1 << 12;
  LL_SPI_EnableNSSPulseMgt(SPI1);
  LL_SPI_Enable(SPI1);
  /* USER CODE END SPI1_Init 2 */

}

/* USER CODE BEGIN 1 */
unsigned char readWriteSPI1(unsigned char txData)
{
	unsigned char rxData;

	LL_SPI_TransmitData8(SPI1, (uint8_t)txData);
	while(LL_SPI_IsActiveFlag_TXE(SPI1) != SET);

	// wait until RXNE = 1
	while(LL_SPI_IsActiveFlag_RXNE(SPI1) != SET);
	// read the rx buff to clear the RXNE flag (garbage)
	rxData = LL_SPI_ReceiveData8(SPI1);

	return rxData;
}

void initCS_Pin(void)
{
	set_cs();
}

void set_cs()
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_5);
}

void reset_cs()
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
}

void initCD_Pin(void)
{

}

void set_cd() {
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void reset_cd() {
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
}

void initRES_Pin()
{

}

void set_res()
{
	LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_0);
}

void reset_res()
{
	LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_0);
}

void device_Unselect()
{

}

void device_Select()
{

}


/* USER CODE END 1 */
