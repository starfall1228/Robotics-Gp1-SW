#include "main.h"
#include "tim.h"


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


//void pwm_init(void) {
//	// init the pwm prescaler value and auto-reload value and start the pwm
//	/* Your code start here */
//	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
//	TIM5->ARR = 59999;
//	TIM5->PSC = 27;
//	TIM5->CCR1 = 1499;
//	/* Your code end here */
//}

uint8_t counter = 0;
void pwm_classwork(void) {
	/* Your code start here */
	switch (counter++) {
	case 0:
		gpio_reset(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		TIM5->CCR1 = 1499; // -90
		break;

	case 1:
		gpio_set(LED1);
		gpio_reset(LED2);
		gpio_set(LED3);
		TIM5->CCR1 = 4499; // 0
		break;

	case 2:
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_reset(LED3);
		TIM5->CCR1 = 7499; // 90
		break;

	default:
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		counter = 0;
	}
	/* Your code end here */
}

void pwm_classwork2(void) {
	/* Your code start here */
	switch (btn_state()) {
	case BTN1_ONLY:
		gpio_reset(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		TIM5->CCR1 = 1499; //-90 degree
		break;

	case BTN2_ONLY:
		gpio_set(LED1);
		gpio_reset(LED2);
		gpio_set(LED3);
		TIM5->CCR1 = 4499; //0 degree
		break;

	case BOTH_BTN:
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_reset(LED3);
		TIM5->CCR1 = 7499; //90 degree
		break;

	default:
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
		counter = 0;
	}
	/* Your code end here */
}


uint32_t last_ticks = 0;
uint32_t last_ticks2 = 0;
uint32_t blink_ticks = 0;
uint8_t status = 0;
uint8_t single_count = 0;
uint8_t double_count = 0;
uint8_t double_fullcount = 0;
uint8_t special_count = 0;
uint8_t counter2 = 0;
uint16_t CCR_1 = 0;
uint16_t CCR_2 = 0;
uint8_t direction = 0;
void pwm_homework(void) {
	/* Your code start here */
//
//	switch(btn_state()) {
//		case BTN2_ONLY:
//			if (single_count/2 > double_count) {
//				if (status == 0) {
//					TIM5->CCR1 = 2834; //40 degree
//					status = 1;
//				}
//				if (status == 1) {
//					if (HAL_GetTick() - last_ticks >= 2000) {
//						gpio_reset(LED1);
//						gpio_reset(LED2);
//						gpio_reset(LED3);
//						gpio_reset(LED4);
//					}
//
//					else if (HAL_GetTick() - last_ticks >= 1500) {
//						gpio_reset(LED1);
//						gpio_reset(LED2);
//						gpio_reset(LED3);
//						gpio_set(LED4);
//					}
//
//					else if (HAL_GetTick() - last_ticks >= 1000) {
//						gpio_reset(LED1);
//						gpio_reset(LED2);
//						gpio_set(LED3);
//						gpio_set(LED4);
//					}
//
//					else if (HAL_GetTick() - last_ticks >= 500) {
//						gpio_reset(LED1);
//						gpio_set(LED2);
//						gpio_set(LED3);
//						gpio_set(LED4);
//					}
//				}
//			}
//
//			break;
//
//		case BTN1_ONLY:
//			if (status == 0) status = 3;
//
//			if (status == 3) {
//				if (HAL_GetTick() - last_ticks >= 3000 && HAL_GetTick() - blink_ticks >= 100) {
//					blink_ticks = HAL_GetTick();
//					gpio_toggle(LED1);
//					gpio_toggle(LED2);
//					gpio_toggle(LED3);
//					gpio_toggle(LED4);
//				}
//			}
//
//			break;
//
//		case ALL_OFF:
//			gpio_set(LED1);
//			gpio_set(LED2);
//			gpio_set(LED3);
//			gpio_set(LED4);
//			switch (status) {
//				case 0: // General Cases
//					last_ticks = HAL_GetTick();
//					TIM5->CCR1 = 1499; // -90 degree
////					if (special_count >= 2) tft_prints(0, 0, "You did twice!");
//
//					break;
//
//				case 1: // double move
//					if (HAL_GetTick() - last_ticks >= 2000) {
//						TIM5->CCR1 = 7499; // 90 degree
//					} else {
//						TIM5->CCR1 = 7499; // 90 degree
//					}
//					last_ticks2 = HAL_GetTick();
//					status = 2;
//
//					break;
//
//				case 2: // reset double move
//					if (HAL_GetTick() - last_ticks2 >= 2000) {
//						double_count++;
//						status = 0;
//					} else if (HAL_GetTick() - last_ticks >= 2000 && HAL_GetTick() - last_ticks2 >= 1000) {
//						double_count++; double_fullcount++;
//						status = 0;
//					}
//
//					break;
//
//				case 3: // Determine Special or Single move
//					if (HAL_GetTick() - last_ticks >= 3000) {
//						CCR_1 = 1499; CCR_2 = 1799;
//						status = 4;
//					} else {
//						status = 5;
//					}
//					counter2 = 0;
//					last_ticks2 = HAL_GetTick();
//
//					break;
//
//				case 4: // Special Move
//					if (HAL_GetTick()-last_ticks2 >= 4000) {
//						special_count++;
//						status = 0;
//					} else if (HAL_GetTick()-last_ticks2 >= 200*counter2) {
//						counter2++;
//
//						if((counter2)%2 == 0) {
//							CCR_1 += 300;
//							TIM5->CCR1 = CCR_1;
//						} else {
//							CCR_2 += 300;
//							TIM5->CCR1 = CCR_2;
//						}
//					}
//
//					break;
//
//				case 5: // Single Move
//					if (HAL_GetTick()-last_ticks2 >= 3000) {
//						single_count++;
//						status = 0;
//					} else if (HAL_GetTick()-last_ticks2 >= 100*counter2) {
//						counter2++;
//						TIM5->CCR1 = 1499+(200*counter2);
//					}
//
//					break;
//
//				}
//	}
	/* Your code end here*/
}
