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

typedef struct PID {
	double kp;
	double ki;
	double kd;
	double setspeed;
	double actual_speed;
	double integral;
	double err;
	double err_last;
	double current;
}PID;




double error_integral = 0;


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


// Implemented for outputting steady speed- alternative approach
// Hi Taimas, please help me to test these 2  functions if you arrive early lol
void init_PID(PID* pid, double setspeed,double kp, double ki, double kd){
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->err=0.0;
	pid->err_last=0.0;
	pid->integral=0.0;
	pid->current = 0;
	pid->setspeed = setspeed;
}

void upp_state_speed(double setspeed,Motor motor, PID* pid, int* last_ticks){
	double cur_vel = abs(get_motor_feedback(motor).vel_rpm);
    pid->setspeed = setspeed;
    pid->err = pid->setspeed - cur_vel;
    pid->integral += pid->err;
    double dt = (HAL_GetTick() - *last_ticks)/1000;
    *last_ticks = HAL_GetTick();
    pid->current = pid->kp*pid->err + pid->ki*pid->integral+pid->kd*(pid->err-pid->err_last);
    pid->err_last=pid->err;
}

//int last_ticks = HAL_GetTick();
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
//    volatile uint32_t last_ticks = 0;

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
    can_init(); // initialize CAN
    cur_err = 500;
    // The rpm of the motor
    double setspeed = 2000;
    double kp = 0.5;
    double ki = 0.01;
    double kd = 0.01;
    PID pid0;
    PID pid1;
    init_PID(&pid0, setspeed,kp,ki, kd);
    init_PID(&pid1, setspeed,kp,ki, kd);
    int last_ticks =HAL_GetTick();
    int temp = HAL_GetTick();
    static const char dat[] = "GGGG";
    uint8_t tim = 10;
    char message[] = "xuys";
    HAL_UART_Transmit(&huart1, (uint8_t*)&dat, sizeof(dat),tim);
    bool rcv = 0;
    int last_mov = HAL_GetTick();
    while(1){
        can_ctrl_loop(); // to continously send/receive data with CAN
			if(!rcv) {
				char message1[10] = {'\0'};
				HAL_UART_Receive(&huart1,(uint8_t*)&message1, sizeof(message1), tim);
				if(message1[0] != '\0') {
					for(int i = 0;i < 4;i++) {
						message[i] = message1[i];
					}
					rcv = 1;
					last_mov = HAL_GetTick();
				}

			}
//        uint32_t tim_dif = HAL_GetTick() - temp;
//             tft_prints(0, 5, "Time dif: %d", HAL_GetTick() - last_ticks);
			can_ctrl_loop(); // to continously send/receive data with CAN
             if (HAL_GetTick() - last_mov <= 2000) {
            	upp_state_speed(setspeed,CAN1_MOTOR0, &pid0,&last_ticks);
				upp_state_speed(setspeed,CAN2_MOTOR0, &pid1,&last_ticks);
            	 set_motor_current(CAN1_MOTOR0, pid0.current);
            	 set_motor_current(CAN2_MOTOR0, -pid1.current);
	 //        	set_motor_current(CAN1_MOTOR1, PID_current_input(600,CAN1_MOTOR1));
	 //            set_motor_current(CAN1_MOTOR0, 700);
			 }else {
				 init_PID(&pid0, setspeed,kp,ki, kd);
				 init_PID(&pid1, setspeed,kp,ki, kd);
				 set_motor_current(CAN1_MOTOR0, 0);
				 set_motor_current(CAN2_MOTOR0, 0);

			 }
//             else if (!btn_read(BTN2)) {
//				upp_state_speed(setspeed,CAN1_MOTOR0, &pid0,&last_ticks);
//				upp_state_speed(setspeed,CAN2_MOTOR0, &pid1,&last_ticks);
//				set_motor_current(CAN1_MOTOR0, -pid0.current);
//				set_motor_current(CAN2_MOTOR0, pid1.current);
//	 //        	set_motor_current(CAN1_MOTOR1, -PID_current_input(600,CAN1_MOTOR1));
//	 //            set_motor_current(CAN1_MOTOR0, -700);
//			 } else {
//				 init_PID(&pid0, setspeed,kp,ki, kd);
//				 init_PID(&pid1, setspeed,kp,ki, kd);
//				 set_motor_current(CAN1_MOTOR0, 0);
//				 set_motor_current(CAN2_MOTOR0, 0);
//	 //            set_motor_current(CAN1_MOTOR1, 0);
//			 }
//        }
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        tft_update(100);
        // printing feedback on TFT
        // should display 50 if implemented correctly
//        tft_prints(0, 0, "MOTOR1 VEL: %d", get_motor_feedback(CAN1_MOTOR0).vel_rpm/60);
//        tft_prints(0, 1, "MOTOR2 VEL: %d", get_motor_feedback(CAN2_MOTOR0).vel_rpm/60);
////        tft_prints(0, 1, "MOTOR ENC: %d", get_motor_feedback(CAN1_MOTOR0).encoder);
        tft_prints(0, 2, "MOTOR CUR: %0.3f", (double)get_motor_feedback(CAN1_MOTOR0).actual_current);
        tft_prints(0, 3, "MOTOR CUR: %0.3f", (double)get_motor_feedback(CAN1_MOTOR0).actual_current);
        tft_prints(0, 4, "pid0_cur: %f", pid0.current);
        tft_prints(0, 5, "pid1_cur: %f", pid1.current);
        tft_prints(0,6,"SS0: %0.3f", pid0.setspeed/60);
        tft_prints(0,7,"SS1: %0.3f", pid1.setspeed/60);
        tft_prints(0,8, "%d", rcv);
        tft_prints(0,9, message);
//        tft_prints(0,4, "l")


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
