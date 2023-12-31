/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "pwm.h"
#include "lcd/lcd.h"
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
    MX_GPIO_Init();
    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM5_Init();
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
  MX_CAN1_Init();
  MX_CAN2_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_TIM5_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
    volatile uint32_t last_ticks = 0;
    volatile uint32_t last_ticks_inc = 0;

    // we turn off all the led & cylinder first
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    gpio_reset(GRIPPER);
    gpio_reset(ELEVATING);
    gpio_reset(SIDE_CYL);
    tft_init(PIN_ON_TOP, BLACK, WHITE, YELLOW, DARK_GREEN);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
    pwm_init();


    tft_force_clear();
	can_init();

	// pre-define constant
	const Motor motorchoice[] = {CAN1_MOTOR0, CAN1_MOTOR1, CAN1_MOTOR2, CAN1_MOTOR3};
	const char pid_text[6][20] = {"kp-up", "kp-down", "ki-up", "ki-down", "kd-up", "kd-down"};
	const char Btn1_text[7][20] = {"Speed Increase", "Speed Decrease", "Speed Test", "Side CYL", "Gripper", "Elevator","Fast Track"};

	// Status of each Btn
	static int btn1_choice = 6;
	enum {kp_increase, kp_decrease, ki_increase, ki_decrease, kd_increase, kd_decrease} k_choice;
	static int target_vel[4] = {0,0,0,0};
	static int velocity = 500;

	//initialize
	k_choice = kp_increase;
	//init_fast_track(motorchoice, target_vel);
	PID_variable_init();
	Reset_dat_init();

	// Hold&click Variable
	static int Btn1_mode = 0;
	static int Btn2_mode = 0;
	static int Btn1_HoldTime = 0;
	static int Btn2_HoldTime = 0;
	static int deltatime = 0;
	static int last_cyl_time = 0;
	static int fast_track_time = 0;


	// varying constant
	static double kp = 9,
				kd = -0.0153,
				ki = 0.0003;


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */




  while (1)
  {

	can_ctrl_loop();
	if (HAL_GetTick() - last_ticks >= 100) {
		led_toggle(LED1);
		last_ticks = HAL_GetTick();
	}
	testing(motorchoice);
	switch(Btn1_mode) {
		// listening
		case (0):
			if (!btn_read(BTN1)) {
				Btn1_mode++;
				Btn1_HoldTime = HAL_GetTick();
			}
		break;
		// Holding
		case (1):
			if (HAL_GetTick() - Btn1_HoldTime < 500) {
				if (btn_read(BTN1)) Btn1_mode = 2;
			}
			else Btn1_mode = 3;
		break;
		//Clicking
		case (2):
			btn1_choice++; btn1_choice %= 7;
			Btn1_mode = 0;
		break;
		//Holding
		case (3):
			switch (btn1_choice) {
				case 0:
					if (HAL_GetTick() - deltatime > 100) {
						deltatime = HAL_GetTick();
						velocity += 50;
					}
				break;
				case 1:
					if (HAL_GetTick() - deltatime > 100) {
						deltatime = HAL_GetTick();
						velocity -= 50;
					}
				break;
				case 2:
					for (int i = 0; i < 4; i++) {
						target_vel[i] = velocity;
					}
				break;
				case 3:
					if (HAL_GetTick() - last_cyl_time > 1000) {
						last_cyl_time = HAL_GetTick();
						gpio_toggle(SIDE_CYL);
					}
				break;
				case 4:
					if (HAL_GetTick() - last_cyl_time > 1000) {
						last_cyl_time = HAL_GetTick();
						gpio_toggle(GRIPPER);
					}
				break;
				case 5:
					if (HAL_GetTick() - last_cyl_time > 1000) {
						last_cyl_time = HAL_GetTick();
						gpio_toggle(ELEVATING);
					}
				break;
				case 6:
					Btn1_mode = 4;
					fast_track_time = HAL_GetTick();
				break;
			}
			if (btn_read(BTN1)) {
				for (int i = 0; i < 4; i++) {
					target_vel[i] = 0;
				}
				Btn1_mode = 0;
			}
		break;

		case (4):
			last_cyl_time = HAL_GetTick();

			for (int i = 0; i < 4; i++) {
				target_vel[i] = 2000;
			}

			if (HAL_GetTick() - fast_track_time > 4500) {
				for (int i = 0; i < 4; i++) {
					target_vel[i] = 0;
				}
				Btn1_mode = 0;
			}
		break;
	}
	switch(Btn2_mode) {
		// listening
		case (0):
			if (!btn_read(BTN2)) {
				Btn2_mode++;
				Btn2_HoldTime = HAL_GetTick();
			}
		break;
		// Holding
		case (1):
			if (HAL_GetTick() - Btn2_HoldTime < 500) {
				if (btn_read(BTN2)) Btn2_mode = 2;
			}
			else Btn2_mode = 3;
		break;
		//Clicking
		case (2):
			k_choice++; k_choice %= 6;
			Btn2_mode = 0;
		break;
		//Holding
		case (3):
			if (btn_read(BTN2)) Btn2_mode = 0;
			if (HAL_GetTick() - last_ticks_inc >= 100) {
				switch (k_choice) {
					case (kp_increase):
						kp *= 1.1;
					break;
					case (kp_decrease):
						kp *= 0.9;
					break;
					case (ki_increase):
						ki *= 1.1;
					break;
					case (ki_decrease):
						ki *= 0.9;
					break;
					case (kd_increase):
						kd *= 1.1;
					break;
					case (kd_decrease):
						kd *= 0.9;
					break;
				}
				last_ticks_inc = HAL_GetTick();
			}
			tft_prints(0, 7, "%0.5f", kp);
			tft_prints(0, 8, "%0.5f", ki);
			tft_prints(0, 9, "%0.5f", kd);
		break;
	}

	ReceiveData(target_vel);

	if(!isAutoTrack) fast_track_time = HAL_GetTick();
	if(isAutoTrack){
		tft_prints(0, 5, "Autotracking");
		autotrack(fast_track_time);
	}

	for (int i = 0; i < 4; i++ ) {
		set_motor_speed(motorchoice[i], target_vel[i], kp, ki, kd, motorchoice);
		//set_motor_current(motorchoice[i], 700);

	}

	tft_prints(0, 0, "time: %d", (int) HAL_GetTick());
	tft_prints(0, 1, "%s     ", Btn1_text[btn1_choice]);
	tft_prints(0, 2, "%s %d", pid_text[k_choice], velocity);
	tft_prints(0, 3, "tar: %d %d ", target_vel[0], target_vel[1]);
	tft_prints(0, 4, "tar: %d %d ", target_vel[2], target_vel[3]);
	tft_update(100);
	SendData(motorchoice);



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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
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
