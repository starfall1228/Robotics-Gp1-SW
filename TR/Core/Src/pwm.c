#include "main.h"
#include "tim.h"
#include "pwm.h"

void pwm_init(void) {
	// init the pwm prescaler value and auto-reload value and start the pwm
	/* Your code start here */

	// PC8
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	TIM3->ARR = 59999;
	TIM3->PSC = 27;
	TIM3->CCR1 = 1499; //-90

	//PA0
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	TIM5->ARR = 59999;
	TIM5->PSC = 27;
	TIM5->CCR1 = 1499; //-90
	/* Your code end here */
}

/*
 * 1499 - -90
 *
 * 4499 - 0
 *
 * 7499 - 90
 *
 * */

void pwm_angle(int angle) {
	TIM3->CCR1 = angle*100/3+4499;
	TIM5->CCR1 = angle*100/3+4499;
}
