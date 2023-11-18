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
#include "can.h"
#include "lcd/lcd.h"
#include "math.h"
#include "gpio.h"
#include "pwm.h"
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#define PI 3.141592654

int mode1 = 0;

uint32_t updateTime = 0;

char dat[30];
char fulldat[30];
char tofdat[60];
int count = 0;
int count_tof = 0;
int* target = NULL;
const Motor* motor_choice = NULL;
int value_Time = 0;
int shifted = 0;
int percent_vel = 100;
const int max_velocity = 1500;
const int fast_track_time = 2000;

int prev_error = 0;
int accu = 0;
int side_cyl_on = 0;

int velocity = max_velocity;


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

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
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

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
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

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
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

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
   // Happy void space
}

void Reset_dat_init() {
	for (int i = 0; i < 30; i++) {
		dat[i] = '\0';
	}
	return;
}

void Reset_tofdat_init() {
	for (int i = 0; i < 60; i++) {
		tofdat[i] = '\0';
	}
}



void shift() {
	shifted = fulldat[2] - '0';
	velocity = max_velocity*((shifted == 0)? 1:0.3)*percent_vel/100;
	Reset_dat_init();
	tft_prints(0, 5, "%s", "Shifting");
	return;
}

void set_tar_velocity(int v1, int v2, int v3, int v4) {
	PID_variable_init();
	*(target+0) = v1*velocity;
	*(target+1) = v2*velocity;
	*(target+2) = v3*velocity;
	*(target+3) = v4*velocity;
	return;
}

void init_fast_track(const Motor* motorchoice, int* targetvel) {
	motor_choice = motorchoice;
	target = targetvel;
}

// 800 - 1550 - 4600 - 5450 (total 3100) r = 6.5 cm
//void fast_track() {
//
//	static const int fast_track_time = 500;
//	const double kp = 9, ki = 0.001, kd = -0.03;
//	int initalTime = HAL_GetTick();
//
//	led_on(LED3);
//	if(HAL_GetTick() - initalTime < fast_track_time ) {
//		for (int i = 0; i < 4; i++ ) {
//			set_motor_speed(*(motor_choice+i), 1000 , kp, ki, kd, motor_choice);
//		}
//
//	}
//	set_tar_velocity(0,0,0,0);
//	led_off(LED3);
//}

void decode_command(int value) {
	switch (value) {
	// Push button
		// Up
		case 20:
			set_tar_velocity(1	,1	,1	,1);
		break;

		// Right
		case 22:
			set_tar_velocity(1	,-1	,1	,-1);
		break;

		// Left
		case 21:
			set_tar_velocity(-1	,1	,-1	,1);
		break;

		// Down
		case 23:
			set_tar_velocity(-1	,-1	,-1	,-1);
		break;

		// Diag-Up-right
		case 24:
			set_tar_velocity(2	,0	,2	,0);
		break;

		// Diag-Up-left
		case 26:
			set_tar_velocity(0	,2	,0	,2);
		break;

		// Diag-Down-right
		case 25:
			set_tar_velocity(0	,-2	,0	,-2);
		break;

		// Diag-Down-left
		case 27:
			set_tar_velocity(-2	,0	,-2	,0);
		break;

		// Rotate Right
		case 28:
			set_tar_velocity(-1	,1	,1	,-1);
		break;

		// Rotate Left
		case 30:
			set_tar_velocity(1	,-1	,-1	,1);
		break;

		// Unpush Button
		// Up
		case 4:
		// Right
		case 6:
		// Left
		case 5:
		// Down
		case 7:
		// Diag-Up-right
		case 8:
		// Diag-Up-left
		case 10:
		// Diag-Down-right
		case 9:
		// Diag-Down-left
		case 11:
		// Rotate Right
		case 12:
		// Rotate Left
		case 14:
		// stop
		case 0:
			set_tar_velocity(0	,0	,0	,0);
			Re
		break;

		// Lift UP
		case 29:
			gpio_on(ELEVATING);
		break;

		// LIFT DOWN
		case 13:
			gpio_off(ELEVATING);
		break;

		// clamp on
		case 31:
			gpio_on(GRIPPER);
		break;

		//clamp off
		case 15:
			gpio_off(GRIPPER);
		break;
	}
}

void end_bit() {
	fulldat[--count] = '\0';

	if (count != 5) {
		count = 0;
		mode1 = 0;
		Reset_dat_init();
		return;
	}

	int value = 0;
	int temp = 16;
	int tempvalue = 0;
	int tempmulti = 100;

	tft_prints(0, 5, "%s ", fulldat);
	led_toggle(LED2);
	//--------------------
	switch (fulldat[0]) {
		case 's':
			shift();
		break;
		case 'p':
			gpio_toggle(SIDE_CYL);
			Reset_dat_init();
		break;
		case 'v':
			for (int i = 3; i < 6; i++) {
				tempvalue += tempmulti * (fulldat[0]-'0');
				tempmulti /= 10;
			}
			percent_vel = tempvalue;
		break;
		case '1':
		case '0':
			for (int i = 0; i < 5; i++) {
				if (fulldat[i] != '0' && fulldat[i] != '1'){
					Reset_dat_init();
					count = 0;
					mode1 = 0;
					return;
				}
				value += (fulldat[i] - '0') * temp;
				temp /= 2;
			}
			tft_prints(0, 5, "%s  %d", fulldat, value);

			Reset_dat_init();

			decode_command(value);
		break;

	}
	count = 0;
	mode1 = 0;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (dat[0] == 't')  {
		value_Time = HAL_GetTick();
		Reset_dat_init();
		return;
	}
	if (huart == &huart1) {
		switch (mode1) {
			// start bit??
			case 0:
				if (dat[0] == 'm') {
					mode1 = 1;
					Reset_dat_init();
				}
			break;

			// detecting...
			case 1:
				fulldat[count++] = dat[0];
				tft_prints(0, 5, "%s  ", fulldat);
				if (dat[0] == 'n') {
					end_bit();
				}
				Reset_dat_init();
			break;
		}
	} else if (huart == &huart2) {
//		static int mode2 = 0;
//
//		tofdat[count_tof++] = dat[0];
//		Reset_dat_init();

		if (tofdat[0] != 'S') {
			Reset_tofdat_init();
			return;
		};

		tft_prints(0, 5, "%s ", tofdat);
		Reset_tofdat_init();
	}

	return;
}


void ReceiveData(int tar_vel[4]) {
	target = tar_vel;
//	tft_prints(0, 5, "%d", HAL_GetTick() - value_Time);
	HAL_UART_Receive_IT(&huart1, (uint8_t*)&dat, sizeof(char) * 1);
	if (HAL_GetTick() - value_Time > 1500) {
		for (int i = 0; i < 4; i++) tar_vel[i] = 0;
		gpio_reset(LED4);
	} else {
		gpio_set(LED4);
	}
//	HAL_UART_Receive_IT(&huart2, (uint8_t*)&tofdat, sizeof(char) * 34);
//	if (HAL_GetTick()-updateTime > 2000) HAL_UART_Receive(&huart1, (uint8_t*)&dat, sizeof(char) * 1)
	return;
}

void SendData(const Motor motorchoice[4]) {
	motor_choice = motorchoice;
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
		char fulldat[50];
		char dat_send[50];
		int i = 0;

		for (; i < 50; i++) {
			fulldat[i] = value%10 + '0';
			value /= 10;
			if (value == 0) break;
		}

		dat_send[i+1] = '\0';
		for (int k = 0; k <= i ; k++) {
			dat_send[k] = fulldat[i-k];
		}
//		tft_prints(0, 5, "%s  ", dat_send);
//		tft_update(100);
		last_Send_Time = HAL_GetTick();
		HAL_UART_Transmit_IT(&huart1, (uint8_t*)&dat_send, sizeof(char)*(i+1));
	}
	return;
}
/* USER CODE END 1 */
