#include "main.h"
#include "lcd/lcd.h"
#include "lcd/lcd_graphics.h"

/** Design ideologies:
 * More small functions >>> Few giant functions
 * Reusability and Extendability is most important (Dont-Repeat-Yourself)
 * then Readability (comments are good, self documenting code is best)
 * No "magic numbers", use defines, constants, variables etc
 * ALWAYS INDENT CORRECTLY
 * No blocking (long while loops, HAL_Delay)
 */

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

ButtonState btn_state(void) {
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

/* Private variables START */

ButtonState state = ALL_OFF;

/* Private variables END */

/* Private function prototypes START */

/* Private function prototypes END */

void gpio_classwork(void) {
    /* Your code start here */
	int last_pressed = HAL_GetTick();
	if (gpio_read(BTN1) == 0){
		HAL_Delay(200);
		if(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0){
			while(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0){
			// LED1 and LED3 are on while LED2 are flashing.
			last_pressed = HAL_GetTick();
			gpio_reset(LED1);
			gpio_reset(LED3);
			while(HAL_GetTick() - last_pressed < 1000){
				gpio_toggle(LED2);
				HAL_Delay(50);
			}
			last_pressed = HAL_GetTick();
			gpio_set(LED1);
			gpio_set(LED2);
			gpio_set(LED3);

			// After 1 second, LED1 and LED3 are flashing while LED2 are on.
			gpio_reset(LED2);
			while(HAL_GetTick() - last_pressed < 1000){
				gpio_toggle(LED1);
				gpio_toggle(LED3);
				HAL_Delay(50);
			}
		}
		}
		gpio_reset(LED1);
		while(gpio_read(BTN1) == 0){

		}
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
	}else if(gpio_read(BTN2) == 0){
		HAL_Delay(200);
		if(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0){
			while(gpio_read(BTN1) == 0 && gpio_read(BTN2) == 0){
			// LED1 and LED3 are on while LED2 are flashing.
			last_pressed = HAL_GetTick();
			gpio_reset(LED1);
			gpio_reset(LED3);
			while(HAL_GetTick() - last_pressed < 1000){
				gpio_toggle(LED2);
				HAL_Delay(50);
			}
			last_pressed = HAL_GetTick();
			gpio_set(LED1);
			gpio_set(LED2);
			gpio_set(LED3);

			// After 1 second, LED1 and LED3 are flashing while LED2 are on.
			gpio_reset(LED2);
			while(HAL_GetTick() - last_pressed < 1000){
				gpio_toggle(LED1);
				gpio_toggle(LED3);
				HAL_Delay(50);
			}
		}
		}
		while(gpio_read(BTN2) == 0){
			gpio_toggle(LED2);
			HAL_Delay(50);
		}
		gpio_set(LED1);
		gpio_set(LED2);
		gpio_set(LED3);
	}
    /* Your code end here */
}

void draw_rectangle(int x, int y, uint16_t color){
	for(int i = x; i < x +50; i++){
		for(int j = y; j < y + 50; j++){

			tft_print_pixel(color, i, j);
		}
	}
}
void tft_classwork(void) {
    /* Your code start here */
	// Print the time elapsed with the format of mm:ss:sssZ where sssZ means millisecond
	int last_pressed = HAL_GetTick();
	int time_elapsed = 0;
//	int temp = last_pressed;

//	int color_index = 0;
	while(1){
		time_elapsed = HAL_GetTick()-last_pressed;
//		temp = HAL_GetTick()-temp;
		//	Print the time elapsed
		tft_prints(0,0,"%d:%d:%d",time_elapsed/(60*1000),time_elapsed%(60*1000)/1000,time_elapsed%1000);

		// Print a 50px times 50px square directly under the elapsed
		// time where its color changes when 1 second passed.
//		drawCircle(50,50,6,color_box[0]);
		int sec = time_elapsed%(60*1000)/1000;
		if(sec%2 == 0 ){
			draw_rectangle(50,50,RED);
//			tft_update(100);
		}else{
			draw_rectangle(50,50,WHITE);
		}


		tft_update(100);

	}
		/* Your code end here */
}

void tutorial2_homework(void) {
    /* Your code start here */

	//While the button is down, print Hello, (Your name) on TFT (@1)
	int state = gpio_read(BTN1);


    if(state == 0){
        tft_prints(0,0,"Hello, SML");
    }
    else if(state == 1){
        gpio_toggle(LED1);
    }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
	// While it is not, flash the LED (at least one LED). (@1)

	// Two actions should not happen simultaneously.
    /* Your code end here*/
}

void tutorial2_homework_l2(void) {
    /* Your code start here */
	int state = gpio_read(BTN1);
	volatile uint32_t last_ticks = 0;
	int last_pressed = HAL_GetTick();
	int temp = HAL_GetTick();
	if (state == 0){
		while(HAL_GetTick() - last_pressed <= 1000){

	        tft_prints(0,0,"Hello, SML");

	        /* USER CODE END WHILE */
	        tft_update(100);
	        /* USER CODE BEGIN 3 */
		}
	}
//	if(state == 1){
	last_pressed = HAL_GetTick();
		while(HAL_GetTick() - last_pressed <= 1000){
			if(HAL_GetTick()- temp >= 50){
				temp = HAL_GetTick();
				gpio_toggle(LED1);
			}
			tft_update(100);
		}
		gpio_set(LED1);

    /* Your code end here*/
}


void tutorial2_homework_bonus(){

	// Create a sprite in the middle of the screen
	int x = MAX_WIDTH/2;
	int y = MAX_HEIGHT/2;
	// It will move to the left for one CHAR_WIDTH when BTN1 is clicked and released
	while(1){
		drawCircle(x,y,10,WHITE);
		if(gpio_read(BTN1) == 0){
			while(1){
				if (gpio_read(BTN1) == 1){
					drawCircle(x,y,10,BLACK);
					x  = x-CHAR_WIDTH;
					drawCircle(x,y,10,WHITE);
					tft_update(100);
					break;
				}

			}
		}
		// move to the right for one CHAR_WIDTH when BTN2 is clicked and released.
		if(gpio_read(BTN2) == 0){
			while(1){
				if (gpio_read(BTN2) == 1){
					drawCircle(x,y,10,BLACK);
					x  = x+CHAR_WIDTH;
					drawCircle(x,y,10,WHITE);
					tft_update(100);
					break;
				}

			}
		}
		tft_update(100);
	}

}

// You can define your helper functions down below
// You can also modify the function definition above as long as your move is reasonable
