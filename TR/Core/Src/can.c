/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    can.c
 * @brief   This file provides code for the configuration
 *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
#include "lcd/lcd.h"
int16_t rm_ctrl_cmd[MAX_CAN_RM * NUM_OF_CAN] = {0};
MotorStats rm_fb_cmd[MAX_CAN_RM * NUM_OF_CAN] = {0};

/* USER CODE END 0 */

CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;

/* CAN1 init function */
void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}
/* CAN2 init function */
void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 3;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_10TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* CAN2 clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1){
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
  else if(canHandle->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_13);

  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void can_filter_enable(CAN_HandleTypeDef *hcan) {
	CAN_FilterTypeDef CAN_FilterConfigStructure;

	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterActivation = ENABLE;
	CAN_FilterConfigStructure.SlaveStartFilterBank = 27;

	CAN_FilterConfigStructure.FilterBank = 0;

	HAL_CAN_ConfigFilter(hcan, &CAN_FilterConfigStructure);
}

void can_filter_disable(CAN_HandleTypeDef *hcan) {
	CAN_FilterTypeDef CAN_FilterConfigStructure;

	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterActivation = DISABLE;
	CAN_FilterConfigStructure.SlaveStartFilterBank = 27;

	CAN_FilterConfigStructure.FilterBank = 0;

	HAL_CAN_ConfigFilter(hcan, &CAN_FilterConfigStructure);
}

void can_init() {
	CAN_FilterTypeDef CAN_FilterConfigStructure;

	CAN_FilterConfigStructure.FilterIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdHigh = 0x0000;
	CAN_FilterConfigStructure.FilterMaskIdLow = 0x0000;
	CAN_FilterConfigStructure.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	CAN_FilterConfigStructure.FilterMode = CAN_FILTERMODE_IDMASK;
	CAN_FilterConfigStructure.FilterScale = CAN_FILTERSCALE_32BIT;
	CAN_FilterConfigStructure.FilterActivation = ENABLE;
	CAN_FilterConfigStructure.FilterBank = 0;

	HAL_CAN_ConfigFilter(&hcan1, &CAN_FilterConfigStructure);
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

	HAL_CAN_ConfigFilter(&hcan2, &CAN_FilterConfigStructure);
	HAL_CAN_Start(&hcan2);
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING);
}

#define GET_MOTOR_FEEDBACK(ptr, data)                                   \
	{                                                                   \
		(ptr)->pos_rad = (uint16_t)((data)[0] << 8 | (data)[1]);        \
		(ptr)->vel_rpm = (uint16_t)((data)[2] << 8 | (data)[3]);        \
		(ptr)->actual_current = (uint16_t)((data)[4] << 8 | (data)[5]); \
		(ptr)->temperature = (data)[6];                                 \
	}

__forceinline void __from_bigendian(uint8_t *in, void *out) {
	uint8_t *o = out;
	*o = *(in + 1);
	*(o + 1) = *in;
}

__forceinline void __get_motor_fb(MotorFeedback *ptr, uint8_t *data) {
	__from_bigendian(data, &(ptr->encoder));
	__from_bigendian(data + 2, &(ptr->vel_rpm));
	__from_bigendian(data + 4, &(ptr->raw_current));
	ptr->temperature = data[6];
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
	CAN_RxHeaderTypeDef rx_header;
	uint8_t rx_data[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rx_header, rx_data);
	uint8_t base_id = rx_header.StdId - CAN_3508_M1_ID;
	if (hcan == &hcan2) {
		base_id += 8;
	}
	MotorFeedback fb;
	__get_motor_fb(&fb, rx_data);
	rm_fb_cmd[base_id].encoder = fb.encoder;
	rm_fb_cmd[base_id].vel_rpm = fb.vel_rpm;
	rm_fb_cmd[base_id].actual_current = (float)fb.raw_current * 20.0f / 16384.0f;
	rm_fb_cmd[base_id].temperature = fb.temperature;
}

void can_transmit(CAN_HandleTypeDef *hcan, uint16_t id, int16_t msg1, int16_t msg2, int16_t msg3, int16_t msg4) {
	CAN_TxHeaderTypeDef tx_header;
	uint8_t data[8];
	uint32_t pTxMailbox;

	tx_header.StdId = id;
	tx_header.IDE = CAN_ID_STD;
	tx_header.RTR = CAN_RTR_DATA;
	tx_header.DLC = CAN_DATA_SIZE;
	tx_header.TransmitGlobalTime = DISABLE;
	data[0] = msg1 >> 8;
	data[1] = msg1;
	data[2] = msg2 >> 8;
	data[3] = msg2;
	data[4] = msg3 >> 8;
	data[5] = msg3;
	data[6] = msg4 >> 8;
	data[7] = msg4;

	if (HAL_CAN_AddTxMessage(hcan, &tx_header, data, &pTxMailbox) == HAL_OK) {
		while (HAL_CAN_IsTxMessagePending(hcan, pTxMailbox))
			;
	}
}

void can_ctrl_loop() {
	/* ======= TX ======= */
	// CAN1
	can_transmit(&hcan1, RM_TX_GRP1_ID, rm_ctrl_cmd[CAN1_MOTOR0], rm_ctrl_cmd[CAN1_MOTOR1], rm_ctrl_cmd[CAN1_MOTOR2],
				 rm_ctrl_cmd[CAN1_MOTOR3]);
	can_transmit(&hcan1, RM_TX_GRP2_ID, rm_ctrl_cmd[CAN1_MOTOR4], rm_ctrl_cmd[CAN1_MOTOR5], rm_ctrl_cmd[CAN1_MOTOR6],
				 rm_ctrl_cmd[CAN1_MOTOR7]);

	// CAN2
	can_transmit(&hcan2, RM_TX_GRP1_ID, rm_ctrl_cmd[CAN2_MOTOR0], rm_ctrl_cmd[CAN2_MOTOR1], rm_ctrl_cmd[CAN2_MOTOR2],
				 rm_ctrl_cmd[CAN2_MOTOR3]);
	can_transmit(&hcan2, RM_TX_GRP2_ID, rm_ctrl_cmd[CAN2_MOTOR4], rm_ctrl_cmd[CAN2_MOTOR5], rm_ctrl_cmd[CAN2_MOTOR6],
				 rm_ctrl_cmd[CAN2_MOTOR7]);

	/* ======= RX ======= */
	HAL_CAN_RxFifo0MsgPendingCallback(&hcan1);
	HAL_CAN_RxFifo0MsgPendingCallback(&hcan2);
}

/* USER FUNCTIONS */

MotorStats get_motor_feedback(Motor tar_motor) { return rm_fb_cmd[tar_motor]; }

void set_motor_current(Motor tar_motor, int16_t tar_current) {
	if (tar_current > 16384) {
		tar_current = 16384;
	} else if (tar_current < -16384) {
		tar_current = -16384;
	}
	rm_ctrl_cmd[tar_motor] = tar_current;
}

int max (int currents[]) {
	int tempmax = currents[0];
	for (int i = 1; i < 500; i++) {
		if (tempmax < currents[i]) tempmax = currents[i];
	}
	return tempmax;
}

int min (int currents[]) {
	int tempmin = currents[0];
	for (int i = 1; i < 500; i++) {
		if (tempmin > currents[i]) tempmin = currents[i];
	}
	return tempmin;
}

int average (int currents[]) {
	int tempsum = currents[0];
	for (int i = 1; i < 500; i++) {
		tempsum += currents[i];
	}
	return tempsum/500;
}


int currents[MAX_NUM_OF_MOTORS][500] = {};
static int indexes[MAX_NUM_OF_MOTORS];
static double error[MAX_NUM_OF_MOTORS];
static double prev_error[MAX_NUM_OF_MOTORS];
static double accu[MAX_NUM_OF_MOTORS];
static double tar_current[MAX_NUM_OF_MOTORS];
//static double last_ticks[MAX_NUM_OF_MOTORS];

int32_t General_PID(double error, double prev_error, double* accu, const double kp, const double ki, const double kd) {
	// variable declarations
	double P_Gain;
	double I_Gain;
	double D_Gain;

	// prevent 0
//	deltatime = (deltatime == 0)? 1 : deltatime; // Failed

	// P
	P_Gain = kp * error;

	// I
	*accu += error;

	I_Gain = ki * (*accu);

	// D
	D_Gain = kd * (error - prev_error) ;

//	last_ticks[cur_motor] = HAL_GetTick();
	double totalOutput = (P_Gain + I_Gain + D_Gain);
	if(totalOutput>=7500){ // Prevent Hum Lun Speed
		PID_variable_init();
		totalOutput = 7500;
	}


	return totalOutput;

}



void PID_variable_init() {
	for (int i = 0; i < MAX_NUM_OF_MOTORS; i++) {
		indexes[i] = 0;
		error[i] = 0;
		prev_error[i] = 0;
		accu[i] = 0;
		tar_current[i] = 0;
//		last_ticks[i] = 0;
	}
}

//for (int i = 0, i < MAX_NUM_OF_MOTORS; i++) {
//	indexes[i]
//}

void set_motor_speed(Motor tar_motor, int16_t tar_vel, const double kp, const double ki, const double kd, const Motor motorchoice[]) {
	static double Cur_vel = 0;
	tar_vel = (tar_motor == motorchoice[1] || tar_motor == motorchoice[2] ) ? -tar_vel: tar_vel;
	tar_vel *= (tar_motor == motorchoice[0] || tar_motor == motorchoice[3]) ? 19/14: 1;
	// collect current velocity
	Cur_vel = get_motor_feedback(tar_motor).vel_rpm;
	error[tar_motor] = tar_vel - Cur_vel;

	// calculate error
	tar_current[tar_motor] = General_PID(error[tar_motor], prev_error[tar_motor], accu+tar_motor,kp,ki,kd);
	prev_error[tar_motor] = error[tar_motor];

	set_motor_current(tar_motor,tar_current[tar_motor]);

	currents[tar_motor][indexes[tar_motor]++ % 500] = Cur_vel;
	return;
}

void testing(const Motor tar_motor[]) {
	int maximum[4];
	int minimum[4];
	int range[4];
	int velocity[4];

	for (int i = 0; i < 4; i++) {
		maximum[i] = max(currents[tar_motor[i]]);
		minimum[i] = min(currents[tar_motor[i]]);
		range[i] = maximum[i] - minimum[i];
		velocity[i] = get_motor_feedback(tar_motor[i]).vel_rpm;

	}

//	tft_prints(0, 6, "%d %d %d %d ", maximum[0], maximum[1], maximum[2], maximum[3]);
//	tft_prints(0, 7, "%d %d %d %d ", minimum[0], minimum[1], minimum[2], minimum[3]);
	tft_prints(0, 6, "%d %d ", velocity[0],velocity[1]);
	tft_prints(0, 7, "%d %d ", velocity[2],velocity[3]);

	tft_prints(0, 8, "1: %d 2: %d ", range[0], range[1]);
	tft_prints(0, 9, "3: %d 4: %d ", range[2], range[3]);
	return;
}

double averagespeed(Motor tar_motor) {
	return average(currents[tar_motor]);
}

/* USER CODE END 1 */
