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

typedef enum {
	True, False
}Bool;

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
ButtonState last_state = ALL_OFF;
uint32_t callcount = 0;
uint32_t TempTime = 0;
uint32_t Timing = 0;
uint32_t UnPressedTime = 0;
uint32_t LastFlashTime = 0;
Bool pressed = False;
Bool PrintText = False;
Bool FlashLight = False;


Bool pressed_Bt1 = False;
Bool pressed_Bt2 = False;
uint32_t Timing_Bt1 = 0;
uint32_t Timing_Bt2 = 0;
uint16_t Smile_x = MAX_WIDTH/2;
uint16_t Smile_y = MAX_HEIGHT/2;

uint8_t Smileface[50][50];

/* Private variables END */

/* Private function prototypes START */
void Smileface_Init(void) {
	for (int i = 0 ; i < 50 ; i++) {
		Smileface[0][i] = 1;
		Smileface[49][i] = 1;
		Smileface[i][0] = 1;
		Smileface[i][49] = 1;
	}

	for (int i = 9 ; i < 25 ; i++) {
		Smileface[14][i] = 1;
		Smileface[15][i] = 1;
		Smileface[16][i] = 1;
		Smileface[34][i] = 1;
		Smileface[35][i] = 1;
		Smileface[36][i] = 1;
	}
}
/* Private function prototypes END */

void gpio_classwork(void) {
    /* Your code start here */
	last_state = state;
	state = btn_state();
	(last_state != state)? callcount = 0: callcount++;
	switch(state) {
		case BOTH_BTN:

			if (callcount <= 1000) {
				//Turns on LED 1 & 3
				gpio_reset(LED1);
				gpio_reset(LED3);
				//flash LED 2
				if (!(callcount % 50)) gpio_toggle(LED2);
			}

			else {
				//Turns on LED 2
				gpio_reset(LED2);
				//flash LED 1 & 3
				if (!(callcount % 50)) {
					gpio_toggle(LED1);
					gpio_toggle(LED3);
				}
			}

			if (callcount > 2000) callcount = 0;

			break;
		case BTN1_ONLY:

			//reset other unrelated button
			gpio_set(LED2);
			gpio_set(LED3);

			// Turns on the LED 1
			gpio_reset(LED1);

			break;
		case BTN2_ONLY:

			//reset other unrelated button
			gpio_set(LED1);
			gpio_set(LED3);

			if (callcount > 50) {
				// Toggles the LED 2
				callcount = 0;
				gpio_toggle(LED2);
			}

			break;
		case ALL_OFF:

			// Turns off the LED 1 & 2 & 3
			gpio_set(LED1);
			gpio_set(LED2);
			gpio_set(LED3);

			break;
	}
    /* Your code end here */
}

void tft_classwork(void) {
    /* Your code start here */
	uint32_t Current_Time = HAL_GetTick();
	tft_prints(0, 0, "%d", (Current_Time/600000) % 10 );
	tft_prints(1, 0, "%d", (Current_Time/60000) % 10 );
	tft_prints(2, 0, ":");
	tft_prints(3, 0, "%d", (Current_Time%60000/10000) % 10);
	tft_prints(4, 0, "%d", (Current_Time%60000/1000) % 10);
	tft_prints(5, 0, ":");
	tft_prints(6, 0, "%d", (Current_Time/100) % 10 );
	tft_prints(7, 0, "%d", (Current_Time/10) % 10 );
	tft_prints(8, 0, "%d", Current_Time % 10 );
	if (!(Current_Time%2000)) {
		for (int i = 20; i <= 70; i++) {
			for (int j = 0; j <= 50; j++) {
				tft_print_pixel(RED,j,i);
			}
		}
	}
	else if (Current_Time%2000 == 1001) {
		for (int i = 20; i <= 70; i++) {
			for (int j = 0; j <= 50; j++) {
				tft_print_pixel(WHITE,j,i);
			}
		}
	}
    /* Your code end here */
}

void tutorial2_homework(void) {
    /* Your code start here */
	Timing = HAL_GetTick();
	if (read_button(BUTTON1)) {
		// close LED 1
		gpio_set(LED1);
		tft_prints(0, 0, "Hello, starfall");
	}

	else if(!read_button(BUTTON1)) {
		// flash LED 1
		if (!(Timing %50)) {
			gpio_toggle(LED1);
		}
	}
    /* Your code end here*/
}

void tutorial2_homework2(void) {
	/* Your code start here */
	TempTime = HAL_GetTick();
	if (read_button(BUTTON1)) {
		if (pressed == False) {
			pressed = True;
			Timing = TempTime;
		}
	}

	else if (!read_button(BUTTON1)) {
		if (pressed == True) {
			pressed = False;
			FlashLight = True;
			UnPressedTime = TempTime;
			if (UnPressedTime-Timing < 200) {
				PrintText = True;
			} else {
				PrintText = False;
			}
		}
	}

	TempTime = HAL_GetTick();
	if (PrintText == True && TempTime - UnPressedTime <= 1000) {
		tft_prints(0, 0, "Hello, starfall");
	}
	TempTime = HAL_GetTick();
	if (FlashLight == True && TempTime - UnPressedTime <= 1000) {
		if ((TempTime - LastFlashTime) >= 50) {
			LastFlashTime = TempTime;
			gpio_toggle(LED1);
		}
	} else {
		gpio_set(LED1);
	}

	/* Your code end here */
}

void tutorial2_homework3(void){
	/* Your code start here */

	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 50; j++) {
			if (Smileface[i][j]) tft_print_pixel(BLACK,i+Smile_x-25,j+Smile_y-25);
		}
	}

	if (read_button(BUTTON1)) {
		if (pressed_Bt1 == False) {
			pressed_Bt1 = True;
			Timing_Bt1 = HAL_GetTick();
		}
	}

	else if (!read_button(BUTTON1)) {
		if (pressed_Bt1 == True) {
			pressed_Bt1 = False;
			if (HAL_GetTick() - Timing_Bt1 <= 200) Smile_x -= CHAR_WIDTH;
		}
	}

	if (read_button(BUTTON2)) {
			if (pressed_Bt2 == False) {
				pressed_Bt2 = True;
				Timing_Bt2 = HAL_GetTick();
			}
		}

	else if (!read_button(BUTTON2)) {
		if (pressed_Bt2 == True) {
			pressed_Bt2 = False;
			if (HAL_GetTick() - Timing_Bt2 <= 200) Smile_x += CHAR_WIDTH;
		}
	}

	for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				if (Smileface[i][j]) tft_print_pixel(WHITE,i+Smile_x-25,j+Smile_y-25);
			}
		}

	/* Your code end here */
}

// You can define your helper functions down below
// You can also modify the function definition above as long as your move is reasonable
