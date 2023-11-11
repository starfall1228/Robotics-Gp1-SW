/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usart.h"
#include "can.h"
#include "math.h"
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USART1 init function */

void MX_USART1_UART_Init(void)
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
/* USART2 init function */

void MX_USART2_UART_Init(void)
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

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}
//
//char *toString(uint8_t *a)
//{
//  char* buffer2;
//  int i;
//
//  buffer2 = malloc(9);
//  if (!buffer2)
//    return NULL;
//
//  buffer2[8] = 0;
//  for (i = 0; i <= 7; i++)
//    buffer2[7 - i] = (((*a) >> i) & (0x01)) + '0';
//
//  puts(buffer2);
//
//  return buffer2;
//}


/* USER CODE BEGIN 1 */
void RecieveData(char dat[10]) {
	char dat[10];
	HAL_UART_Receive(&huart1, (uint8_t*)&dat, sizeof(char) * 10, 10);
	int value = 0;
	int temp = 16;
	for (int i = 0; i < 5; i++) {
		value += (dat[i] - '0') * temp;
		temp /= 2;
	}
	switch (value) {
	// Push button
		// Up
		case 20:

		break;

		// Right
		case 22:

		break;

		// Left
		case 21:

		break;

		// Down
		case 23:

		break;

		// Diag-Up-right
		case 24:

		break;

		// Diag-Up-left
		case 26:

		break;

		// Diag-Down-right
		case 25:

		break;

		// Diag-Down-left
		case 27:

		break;

	// Unpush Button
		// Up
		case 4:

		break;

		// Right
		case 6:

		break;

		// Left
		case 5:

		break;

		// Down
		case 7:

		break;

		// Diag-Up-right
		case 8:

		break;

		// Diag-Up-left
		case 10:

		break;

		// Diag-Down-right
		case 9:

		break;

		// Diag-Down-left
		case 11:

		break;
	
	// stop
		case 0:
			
		break;
		
		default:
			
		break;
	}

}

void SendData(const Motor motorchoice[4]) {
	static uint32_t last_Send_Time = 0;
	if (HAL_GetTick() - last_Send_Time > 1000) {
		double avg_M1 = averagespeed(motorchoice[0]);
		double avg_M2 = averagespeed(motorchoice[1]);
		double avg_M3 = averagespeed(motorchoice[2]);
		double avg_M4 = averagespeed(motorchoice[3]);

		double ver = (avg_M1+avg_M2+avg_M3+avg_M4) * 0.7071;
		double hor = (avg_M1+avg_M3-avg_M2-avg_M4) * 0.7071;

		uint32_t value = ver*ver + hor*hor;
		value = (uint32_t) sqrt(value);
		char tempdat[50];
		char dat[50];
		int i = 0;

		for (; i < 50; i++) {
			tempdat[i] = value%10 + '0';
			value /= 10;
			if (value == 0) break;
		}

		dat[i+1] = '\0';
		for (int k = 0; k <= i ; k++) {
			dat[k] = tempdat[i-k];
		}
		tft_prints(0, 5, "%s  ", dat);
		tft_update(100);
		last_Send_Time = HAL_GetTick();
		HAL_UART_Transmit (&huart1, (uint8_t*)&dat, sizeof(char)*(i+1), 0xFFFF);
	}
}
/* USER CODE END 1 */
