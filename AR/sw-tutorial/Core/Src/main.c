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

void pwm_init(void); //add this line!
void pwm_classwork(void); //add this line!
void pwm_homework(void); //add this line!

// Function to be used when the robot start from the left side, traveling the topmost path
void path_1(){

}

// Function to be used when the robot start from the left side, traveling to bottommost path
void path_2(){

}

// Function to be called when the robot starts from the right side, traveling to the second topmost path
void path_3(){

}
// function to be called when the robot starts from the right side,
//traveling to the second bottommost path

void path_4(){

}

double error_integral = 0;
double rpm_error(double target_rpm, Motor motor){
	return target_rpm-get_motor_feedback(motor).vel_rpm;
}

double rpm_differential_error(double target_rpm,Motor motor, double dt){
	double e1 = rpm_error(target_rpm, motor);
	double e2;
	int last_ticks = HAL_GetTick();

	while(HAL_GetTick() - last_ticks <= dt*1000){

	}
	e2 = rpm_error(target_rpm,motor);
	return (e2 - e1)/dt;

}

double rpm_integrational_error(double target_rpm, Motor motor, double dt ){
	// using HAL library to sum up
	error_integral += dt*rpm_error(target_rpm,motor);
	return error_integral;
}


// We define current as the input and rpm as the output in this case
double PID_current_input(int target_rpm,Motor motor){
	double kp = 2;
	double ki = 0;
	double kd = 0;
	double dt = 0.2;
	return kp*rpm_error(target_rpm,motor)
			+ki* rpm_integrational_error(target_rpm,motor, dt)
			+kd*rpm_differential_error(target_rpm,motor, dt);

}

double last_vel = 0,dif_err = 0,cur_err = 0;
const double PI = 3.146,R = 3.5;
void upd_state(double dt, Motor motor) {
	double cur_vel = get_motor_feedback(motor).vel_rpm / 60.0;
	double new_err = cur_err - dt * 2.0 * PI * R * cur_vel;
	double k_temp = cur_vel;
//	error_integral += ((cur_vel - last_vel) / (2.0 * dt)) * (cur_vel * cur_vel - last_vel * last_vel) + last_vel * dt;
	error_integral += ((new_err - cur_err) / 2.0) * dt;
	dif_err = k_temp;
	last_vel = cur_vel;
	cur_err = new_err;
}
double PID_current_input2(double target_dist,Motor motor, double dt) {
	dt /= 100000.0;
	double kp = 2;
	double ki = 0.0001;
	double kd = 1;
	upd_state(dt, motor);
	if(cur_err < 0)return 0;
	return kp * cur_err + ki * error_integral + kd *  dif_err;
}
int main(void) {
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
    MX_CAN1_Init();
    MX_CAN2_Init();
    MX_USART1_UART_Init();
    MX_DMA_Init();
    MX_USART2_UART_Init();
    MX_TIM5_Init();
    /* USER CODE BEGIN 2 */
    volatile uint32_t last_ticks = 0;

    // we turn off all the led first
    led_off(LED1);
    led_off(LED2);
    led_off(LED3);
    led_off(LED4);
    tft_init(PIN_ON_TOP, BLACK, WHITE, YELLOW, DARK_GREEN);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
//    tft_force_clear();

    pwm_init();
    can_init();
    cur_err = 500;
    while(1){
        can_ctrl_loop(); // to continously send/receive data with CAN
        uint32_t tim_dif = HAL_GetTick() - last_ticks;
        if (tim_dif >= 200) {
            tft_prints(0, 6, "Hello World!");
            led_toggle(LED1);
             led_toggle(LED2);
             led_toggle(LED3);
             led_toggle(LED4);

//             tft_prints(0, 5, "Time dif: %d", HAL_GetTick() - last_ticks);
             if (!btn_read(BTN1)) {
				set_motor_current(CAN1_MOTOR0, PID_current_input2(1000,CAN1_MOTOR0, tim_dif));
	 //        	set_motor_current(CAN1_MOTOR1, PID_current_input(600,CAN1_MOTOR1));
	 //            set_motor_current(CAN1_MOTOR0, 700);
			 } else if (!btn_read(BTN2)) {
				set_motor_current(CAN1_MOTOR0, -PID_current_input2(1000,CAN1_MOTOR0, tim_dif));
	 //        	set_motor_current(CAN1_MOTOR1, -PID_current_input(600,CAN1_MOTOR1));
	 //            set_motor_current(CAN1_MOTOR0, -700);
			 } else {
	 //        	error_integral = 0;
				 set_motor_current(CAN1_MOTOR0, 0);
	 //            set_motor_current(CAN1_MOTOR1, 0);
			 }
            last_ticks = HAL_GetTick();
        }
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        tft_update(100);
        // printing feedback on TFT
        tft_prints(0, 0, "MOTOR VEL: %d", get_motor_feedback(CAN1_MOTOR0).vel_rpm);
        tft_prints(0, 1, "MOTOR ENC: %d", get_motor_feedback(CAN1_MOTOR0).encoder);
        tft_prints(0, 2, "MOTOR CUR: %0.3f", (double)get_motor_feedback(CAN1_MOTOR0).actual_current);
        tft_prints(0, 3, "CUR_ERR: %f", cur_err);
//        tft_prints(0, 2, "MOTOR CUR: %0.3f", get_motor_feedback(CAN1_MOTOR0).actual_current);
        // control the motor with the buttons
    }



    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
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