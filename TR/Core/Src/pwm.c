#include "main.h"
#include "tim.h"
#include "pwm.h"

void pwm_init(void) {
	// init the pwm prescaler value and auto-reload value and start the pwm
	/* Your code start here */
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	TIM5->ARR = 59999;
	TIM5->PSC = 27;
	TIM5->CCR1 = 1499;
	/* Your code end here */
}
