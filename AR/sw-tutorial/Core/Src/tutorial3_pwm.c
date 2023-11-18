#include "main.h"
#include "tim.h"
#include "lcd/lcd.h"
#include "lcd/lcd_graphics.h"

/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */

// Enums are always good
typedef enum {
    BOTH_BTN, BTN1_ONLY, BTN2_ONLY, ALL_OFF,
} ButtonState;

typedef enum {
    BUTTON1, BUTTON2,

    //This trick means the NumButtons will equal how many buttons there are (how many enum values)
    //This works as long as the enum values are simple (count up from 0)
    NUM_BUTTON,
} Button;

typedef enum {
    LED1, LED2, LED3, LED4, NUM_LED,
} LED;

/**
 * @brief read the button state
 * return 1 if the button is pressed;
 * return 0 if the button is released;
 */
static inline uint8_t read_button(Button btn) {
    switch (btn) {
    case BUTTON1:
        return !btn_read(BTN1);
    case BUTTON2:
        return !btn_read(BTN2);
    default:
        return 0;
    }
}

static ButtonState btn_state(void) {
    if (read_button(BUTTON1) && read_button(BUTTON2)) {
        return BOTH_BTN;
    } else if (read_button(BUTTON1)) {
        return BTN1_ONLY;
    } else if (read_button(BUTTON2)) {
        return BTN2_ONLY;
    } else {
        return ALL_OFF;
    }
}

uint8_t tft_update(uint32_t period);
void pwm_init(void) {
	// init the pwm prescaler value and auto-reload value and start the pwm
	/* Your code start here */
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
	TIM5->ARR = 59999;
	TIM5->PSC = 27;
	TIM5->CCR1 = 1499;
	/* Your code end here */
}

void pwm_classwork(void) {
	/* Your code start here */
//	pwm_init();
	TIM5->CCR1 = 1;
	int last_tick = HAL_GetTick();
	while(HAL_GetTick() - last_tick < 1000){

	}
	last_tick = HAL_GetTick();
	while(1){
		if(gpio_read(BTN1) == 0){
			last_tick = HAL_GetTick();
			TIM5->CCR1 = 3;
			break;
		}
	}
	while(HAL_GetTick() - last_tick < 1000){

	}
	last_tick = HAL_GetTick();

	while(1){
		if(gpio_read(BTN1) == 0){
			last_tick = HAL_GetTick();
			TIM5->CCR1 = 5;
			break;
		}
	}
	while(HAL_GetTick() - last_tick < 1000){

	}
//	TIM5->CCR1 = 5;

//	TIM5->CCR1 = 34; //set the compare value of timer5 channel1
//	TIM5->CCR1 = 170;
	/* Your code end here */
}

//const double PI =3.141592654;
volatile uint32_t CCRForAngle(double new_angle, double full_cycle) {
    double need = new_angle / 180.0;
    double time = 0.5 + need * (full_cycle / 10.0);
    // time * 1000.0 / 20.0
    return (uint32_t)(time * (1000.0 / full_cycle));
}


double full_cycle = 20; // should be outside of the loop
double cur_angle = 0; //should be outside of the loop

void reset_servo(){
	if(TIM5->CCR1 <=  4499){

			TIM5->CCR1 = 4499;

	}else{
			TIM5->CCR1 = 1499;
	}
}
void servo_turn( ) {
//    double full_cycle = 20; // should be outside of the loop
//    double cur_angle = 0; //should be outside of the loop
//	switch (counter++) {
//	case 0:
//		gpio_reset(LED1);
//		gpio_set(LED2);
//		gpio_set(LED3);
//		TIM5->CCR1 = 1499; // -90
//		break;
//
//	case 1:
//		gpio_set(LED1);
//		gpio_reset(LED2);
//		gpio_set(LED3);
//		TIM5->CCR1 = 4499; // 0
//		break;
//
//	case 2:
//		gpio_set(LED1);
//		gpio_set(LED2);
//		gpio_reset(LED3);
//		TIM5->CCR1 = 7499; // 90
//		break;
//
//	default:
//		gpio_set(LED1);
//		gpio_set(LED2);
//		gpio_set(LED3);
//		counter = 0;
//	}

}


void single_move(void){

	// It starts at 0deg, moves towards the target and ends at 180deg (of the opposite side).
	if(TIM5->CCR1 != 5){
		TIM5->CCR1 = 5;
		HAL_Delay(1000);
	}else{
		TIM5->CCR1 = 1;
		HAL_Delay(1000);
	}

}


//void double_move(int*num_of_non_fully_double_move, int*num_of_fully_double_move){
//	// First, it starts at 0 deg and pauses at 40 deg (charging for 2 seconds)
//	// until the player releases the Double Move button.
//
//	int time_used = 0;
//	TIM5->CCR1 = 1;
//	HAL_Delay(1000);
//	TIM5->CCR1 = 2;
//	HAL_Delay(1000);
//	int last_tick = HAL_GetTick();
//	while(gpio_read(BTN2) == 0){
//		time_used = HAL_GetTick()- last_tick;
//		// LED lighting
//		if(HAL_GetTick()- last_tick >= 500 && HAL_GetTick()- last_tick < 1000){
//			gpio_reset(LED1);
//		}else if(HAL_GetTick()- last_tick >= 1000 && HAL_GetTick()- last_tick < 1500){
//			gpio_reset(LED2);
//		}else if(HAL_GetTick()- last_tick >= 1500 && HAL_GetTick()- last_tick < 2000){
//			gpio_reset(LED3);
//		}else if(time_used>2000){
//			gpio_reset(LED4);
//		}
//	}
//	gpio_set(LED1);
//	gpio_set(LED2);
//	gpio_set(LED3);
//	gpio_set(LED4);
//	if(time_used >= 500 && time_used < 1000){
//		num_of_non_fully_double_move++;
//		TIM5->CCR1 = 5;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 1;
//		HAL_Delay(1000);
//	}else if(time_used >= 1000 && time_used < 1500){
//		num_of_non_fully_double_move++;
//		TIM5->CCR1 = 5;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 1;
//		HAL_Delay(1000);
//	}else if(time_used >= 1500 && time_used < 2000){
//		num_of_non_fully_double_move++;
//		TIM5->CCR1 = 5;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 1;
//		HAL_Delay(1000);
//	}else {
//		num_of_fully_double_move++;
//		TIM5->CCR1 = 5;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 1;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 5;
//		HAL_Delay(1000);
//		TIM5->CCR1 = 1;
//		HAL_Delay(1000);
//	}
//}

void special_move(void){
	int last_pressed = HAL_GetTick();
	while(HAL_GetTick()-last_pressed < 10000){
		TIM5->CCR1 = 1;
		gpio_reset(LED1);
		HAL_Delay(1000);
		TIM5->CCR1 = 2;
		gpio_reset(LED2);
		HAL_Delay(1000);
		TIM5->CCR1 = 3;
		gpio_reset(LED3);
		HAL_Delay(1000);
		gpio_reset(LED4);
		TIM5->CCR1 = 5;
		HAL_Delay(1000);
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		gpio_set(LED4);
	}

}

void pwm_homework(void) {
	/* Your code start here */
	int num_of_single_move = 0;
	int num_of_non_fully_double_move = 0;
	int num_of_fully_double_move = 0;
	int single_point = 0;
	int double_point = 0;
	int num_of_special_move = 0;
	int special_move_point =0;
	TIM5->CCR1 = 1;
//	tft_prints(0,0,"SM: %d", num_of_single_move);
//	tft_prints(0,2,"Non-fully charged double move: %d", num_of_non_fully_double_move);
//	tft_prints(0,4,"FCD: %d", num_of_fully_double_move);
//	tft_prints(0,6,"SPM: %d", num_of_special_move);
//	tft_update(100);
	while(1){
		tft_prints(0,0,"Single move: %d", num_of_single_move);
		tft_prints(0,2,"Non-fully charged");
		tft_prints(0,3,"double move: %d", num_of_non_fully_double_move);
		tft_prints(0,5,"Fully charged");
		tft_prints(0,6,"double move: %d", num_of_fully_double_move);
		tft_prints(0,8,"Special move: %d", num_of_special_move);
		tft_update(100);
		if(gpio_read(BTN1) == 0){
			HAL_Delay(100);
			if(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0 && double_point > 0&&num_of_fully_double_move%2 == 0 && num_of_fully_double_move != 0){
				while(gpio_read(BTN1) == 0 || gpio_read(BTN2) == 0){

				}
				num_of_special_move++;
				special_move();
				tft_prints(0,6,"Special move: %d", num_of_special_move);
			}else{
				single_move();
				num_of_single_move++;
				if(num_of_single_move%2 == 0){
					single_point++;
				}
				tft_prints(0,0,"Single move: %d", num_of_single_move);
			}

		}else if(gpio_read(BTN2) == 0 && single_point > 0 ){
			HAL_Delay(100);
			if(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0 && num_of_fully_double_move%2 == 0 && num_of_fully_double_move != 0){
				while(gpio_read(BTN1) == 0 || gpio_read(BTN2) == 0){

				}
				special_move();
				num_of_special_move++;
				tft_prints(0,6,"Special move: %d", num_of_special_move);
			}else {
				single_point--;
				int time_used = 0;
				HAL_Delay(1000);
				int last_tick = HAL_GetTick();
				while(gpio_read(BTN2) == 0){
					time_used = HAL_GetTick()- last_tick;
					// LED lighting
					if(HAL_GetTick()- last_tick >= 500 && HAL_GetTick()- last_tick < 1000){
						gpio_reset(LED1);
					}else if(HAL_GetTick()- last_tick >= 1000 && HAL_GetTick()- last_tick < 1500){
						gpio_reset(LED2);
					}else if(HAL_GetTick()- last_tick >= 1500 && HAL_GetTick()- last_tick < 2000){
						gpio_reset(LED3);
					}else if(time_used>=2000){
						if(TIM5->CCR1 == 5){
							TIM5->CCR1 = 4;
						}else{
							TIM5->CCR1 = 2;
						}
						gpio_reset(LED4);

					}
				}
				gpio_set(LED1);
				gpio_set(LED2);
				gpio_set(LED3);
				gpio_set(LED4);
				if(time_used >= 500 && time_used < 1000){
					num_of_non_fully_double_move++;
					if(TIM5->CCR1 == 5){
						TIM5->CCR1 = 1;
						HAL_Delay(1000);
					}else {
						TIM5->CCR1 = 5;
						HAL_Delay(1000);
					}

				}else if(time_used >= 1000 && time_used < 1500){
					num_of_non_fully_double_move++;
					if(TIM5->CCR1 == 5){
						TIM5->CCR1 = 1;
						HAL_Delay(1000);
					}else{
						TIM5->CCR1 = 5;
						HAL_Delay(1000);
					}
				}else if(time_used >= 1500 && time_used < 2000){
					num_of_non_fully_double_move++;
					if(TIM5->CCR1 == 5){
						TIM5->CCR1 = 1;
						HAL_Delay(1000);
					}else{
						TIM5->CCR1 = 5;
						HAL_Delay(1000);
					}
				}else if(time_used >= 2000){
					num_of_fully_double_move++;
					if(TIM5->CCR1 == 4){
						TIM5->CCR1 = 1;
						HAL_Delay(1000);
						TIM5->CCR1 = 2;
						HAL_Delay(1000);
						TIM5->CCR1 = 5;
					}else{
						TIM5->CCR1 = 5;
						HAL_Delay(1000);
						TIM5->CCR1 = 4;
						HAL_Delay(1000);
						TIM5->CCR1 = 1;
						HAL_Delay(1000);
					}
					if(num_of_fully_double_move%2 == 0 && num_of_fully_double_move != 0){
						special_move_point++;
					}
				}
				double_point++;
			}


		}else if(special_move_point > 0){
			special_move();
			num_of_special_move++;
			special_move_point--;
//			tft_prints(0,6,"Special move: %d", num_of_special_move);
		}

	}


	/* Your code end here*/
}
