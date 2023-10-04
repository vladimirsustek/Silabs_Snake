/*
 * Porting layer for a snake game (snake_functions)
 *
 * port.c
 *
 *  Created on: Oct 1, 2023
 *      Author: Vladimir Sustek
 *      https://github.com/vladimirsustek
 */
#include "snake_port.h"

#include "glib.h"
#include "dmd.h"
#include "sl_status.h"
#include "sl_board_control.h"
#include "sl_assert.h"
#include "sl_sleeptimer.h"
#include "sl_joystick.h"
#include "nvm3_generic.h"
#include "nvm3_default.h"

#define SNAKE_SERVER_PORT	(uint16_t)(8000u)
#define UPPER_CASE_IDX (uint32_t)(6u)
#define ALLOWED_LETTERS_LNG (uint32_t)(12u)

#define SCORE_FLASH_OBJ (uint32_t)0x5A000000

#define TO_LOWERCASE(x) (x = (x >= 'a') ? (x-('a'-'A')):(x))
#define IS_ALOWED_LETTER(x) ((x) == 'W'||(x) == 'A'||(x) == 'S'||(x) == 'D'||\
    (x) == 'P'||(x) == 'Q'||(x) == 'w'||(x) == 'a'|| (x) == 's'||(x) == 'd'||\
    (x) == 'p'||(x) == 'q')

/* Randomizer seed */
static uint16_t gRandSeed = 0xABCD;

/* variable for controlling snake's direction */
static char gKeyBoardButton;

static GLIB_Context_t glibContext;
static int currentLine = 0;

static sl_joystick_t jHandle;
static sl_joystick_position_t jPosition =
{ 0 };

/* wrapper around actual control implementation - start */
static void platform_control_init(void)
{
	jHandle.pin = 2;
	jHandle.port = gpioPortD;
	jHandle.state = 0;

	sl_joystick_init(&jHandle);
	sl_joystick_start(&jHandle);
}

/* wrapper around actual tft display init */
void platform_display_init(void)
{
	uint32_t status;

	status = sl_board_enable_display();

	/* Initialize the DMD support for memory lcd display */
	status = DMD_init(0);
	EFM_ASSERT(status == DMD_OK);

	/* Initialize the glib context */
	status = GLIB_contextInit(&glibContext);
	EFM_ASSERT(status == GLIB_OK);

	glibContext.backgroundColor = White;
	glibContext.foregroundColor = Black;

	GLIB_clear(&glibContext);
	GLIB_setFont(&glibContext, (GLIB_Font_t*) &GLIB_FontNarrow6x8);
}

/**
 * @brief  Function to set the gKeyBoardButton
 *
 * @note This function must be called in controlling callback or ISR
 *       function, because it directly stores into gKeyBoardButton
 *       which is further used by platform_get_control()
 *
 * @param  None
 * @retval None
 */
void platform_snake_set_control(char c)
{
	gKeyBoardButton = c;
}

/**
 * @brief  Initializes the randomizer's necessary blocks (platform dependent).
 *
 * @note   Implementation of this function must cover all necessary steps to
 *         run randomizing function. For embedded, the standard rand() function
 *         is not compilable, therefore LSFR mechanism must be used. In this
 *         case, the LFSR base seed is acquired as a noise ADC channel sample.
 *
 * @param  None
 * @retval None
 */
void platform_init_randomizer(void)
{
	/* This randomizer is based on ADC noise
	 * as a LFSR seed number */
}

/**
 * @brief  Prepare hardware (especially screen) for a new game.
 *
 * @note   Implementation of this function cleans and prepares all necessary
 *         platform dependent steps for a new game - e.g. clean screen.
 *
 * @param  None
 * @retval None
 */
void platform_refresh_hw(void)
{
	GLIB_clear(&glibContext);
	DMD_updateDisplay();
}

/**
 * @brief  Draw a rectangle 'cell' into position x, y.
 *
 * @note   Function implementation fits into display ARENA_MAX_X, ARENA_MAX_X.
 *         Adjustment may be for a platform needed. See Snake port.h MACROs.
 *
 * @param x - coordination limited by ARENA_MAX_X.
 * @param y - coordination limited by ARENA_MAX_Y.
 * @retval None
 */
void platform_drawCell(uint16_t x, uint16_t y)
{
	const uint32_t black_color = 0u;
	const uint32_t white_color = 0xFFFFFFFFu;
	for (uint16_t idx = x * CELL_SIZE; idx < x * CELL_SIZE + CELL_SIZE ; idx++)
	{
		for (uint16_t idy = y * CELL_SIZE; idy < y * CELL_SIZE + CELL_SIZE ;
				idy++)
		{
			if ((idx == x * CELL_SIZE )
					|| (idx == x * CELL_SIZE + CELL_SIZE - 1)
					|| (idy == y * CELL_SIZE )
					|| (idy == y * CELL_SIZE + CELL_SIZE - 1))
			{
				GLIB_drawPixelColor(&glibContext, idx, idy, black_color);
			}
			else
			{
				GLIB_drawPixelColor(&glibContext, idx, idy, white_color);
			}
		}
	}
	DMD_updateDisplay();
}

/**
 * @brief  Erase a rectangle 'cell' of position x, y.
 *
 * @note   Function implementation fits into display ARENA_MAX_X, ARENA_MAX_X.
 *         Adjustment may be for a platform needed. See Snake port.h MACROs.
 *
 * @param x - coordination limited by ARENA_MAX_X.
 * @param y - coordination limited by ARENA_MAX_Y.
 * @retval None
 */
void platform_eraseCell(uint16_t x, uint16_t y)
{
	const uint32_t white_color = 0xFFFFFFFF;
	for (uint16_t idx = x * CELL_SIZE; idx < x * CELL_SIZE + CELL_SIZE ; idx++)
	{
		for (uint16_t idy = y * CELL_SIZE; idy < y * CELL_SIZE + CELL_SIZE ;
				idy++)
		{
			if ((idx == x * CELL_SIZE )
					|| (idx == x * CELL_SIZE + CELL_SIZE - 1)
					|| (idy == y * CELL_SIZE )
					|| (idy == y * CELL_SIZE + CELL_SIZE - 1))
			{
				GLIB_drawPixelColor(&glibContext, idx, idy, white_color);
			}
		}
	}
	DMD_updateDisplay();
}

/**
 * @brief  Draw a circle 'food' into position x, y (green border and filling).
 *
 * @note   Function implementation fits into display ARENA_MAX_X, ARENA_MAX_X.
 *         Adjustment may be for a platform needed. See Snake port.h MACROs.
 *
 * @param x - coordination limited by FOOD_MAX_X.
 * @param y - coordination limited by FOOD_MAX_Y.
 * @retval None
 */
void platform_drawFood(uint16_t x, uint16_t y)
{

	const uint32_t black_color = 0u;
	for (uint16_t idx = x * CELL_SIZE; idx < x * CELL_SIZE + CELL_SIZE ; idx++)
	{
		for (uint16_t idy = y * CELL_SIZE; idy < y * CELL_SIZE + CELL_SIZE ;
				idy++)
		{
			GLIB_drawPixelColor(&glibContext, idx, idy, black_color);
		}
	}
	DMD_updateDisplay();

}

/**
 * @brief  Erase a circle 'food' of position x, y (black border and filling).
 *
 * @note   Function implementation fits into display ARENA_MAX_X, ARENA_MAX_X.
 *         Adjustment may be for a platform needed. See Snake port.h MACROs.
 *
 * @param x - coordination limited by FOOD_MAX_X.
 * @param y - coordination limited by FOOD_MAX_Y.
 * @retval None
 */
void platform_eraseFood(uint16_t x, uint16_t y)
{
	const uint32_t white_color = 0xFFFFFFFF;
	for (uint16_t idx = x * CELL_SIZE; idx < x * CELL_SIZE + CELL_SIZE ; idx++)
	{
		for (uint16_t idy = y * CELL_SIZE; idy < y * CELL_SIZE + CELL_SIZE ;
				idy++)
		{
			GLIB_drawPixelColor(&glibContext, idx, idy, white_color);
		}
	}
	DMD_updateDisplay();

}

/**
 * @brief  Initialize all needed peripherals for a snake game
 *
 * @note   For an embedded implementation with a display randomizer,
 *         control and display initialization is needed.
 *
 * @param None
 * @retval None
 */
void platform_init(void)
{
	platform_init_randomizer();
	platform_control_init();
	platform_display_init();
}

/**
 * @brief  Function to randomize a number - LFSR implemenation
 *
 * @note   This function is intended to be used in embedded MCU
 *         application, however for a PC implementation the rand()
 *         may work better (no seed handling needed).
 *
 *         e.g. min + rand() / (RAND_MAX / (max - min + 1) + 1);
 *
 * @param str - string to be printed on the display
 * @param length - length of the string
 * @retval randomized number
 */
uint16_t platform_randomize(void)
{
	uint16_t lsb;

	lsb = gRandSeed & 1;
	gRandSeed >>= 1;
	if (lsb == 1)
	{
		gRandSeed ^= 0xB400u;
	}

	return gRandSeed;
}

/**
 * @brief  Function to get 1ms tick - used for blocking delay
 *
 * @note   For some platform it is native, for others is needed
 *         to created and start free running 1ms timer
 *  *
 * @param ms - tick in milliseconds
 * @retval None
 */
uint16_t platform_msTickGet(void)
{
	return sl_sleeptimer_get_tick_count();
}

/**
 * @brief  Function to be used as a fatal error signaling
 *
 * @note   Not implemented yet
 *
 * @param None
 * @retval None
 */
void platform_fatal(void)
{
	while (1)
		;
}

/**
 * @brief  Function to set snake's direction
 *
 * @note   Function relies on the extKeyBoardButton and then casts
 *         this value into any direction/pause/quit of the snake.
 *         Function natively prevents change of 180° (LEFT-RIGHT)
 *
 * @param snake - pointer to a snake structure
 * @retval None
 */
void platform_get_control(snake_t *snake)
{
	snake_dir_e direction = 0;

	sl_joystick_get_position(&jHandle, &jPosition);

	if (jPosition != JOYSTICK_NONE)
	{
		switch (jPosition)
		{
		case JOYSTICK_C:
			platform_snake_set_control('P');
			break;
		case JOYSTICK_N:
			platform_snake_set_control('W');
			break;
		case JOYSTICK_E:
			platform_snake_set_control('D');
			break;
		case JOYSTICK_S:
			platform_snake_set_control('S');
			break;
		case JOYSTICK_W:
			platform_snake_set_control('A');
			break;
		}
		direction = (snake_dir_e) TO_LOWERCASE(gKeyBoardButton);
	}
	else
	{
		/* this value should be set by platform_snake_set_control */
		direction = (snake_dir_e) TO_LOWERCASE(gKeyBoardButton);

		if (direction == 0)
		{
			return;
		}
		if (!IS_ALOWED_LETTER(direction))
		{
			return;
		}
	}

	/* reset the value as it has been already parsed */
	platform_snake_set_control(0);

	/* If received character is not a known function, do pause and save previous state */
	if ((direction != LEFT) && (direction != RIGHT) && (direction != UP)
			&& (direction != DOWN) && (direction != PAUSE)
			&& (direction != QUIT))
	{
		snake->prev_direction = snake->direction;
		snake->direction = PAUSE;
	}
	else
	{
		/* If characters is a pause*/
		if (direction == PAUSE)
		{
			if (snake->direction != PAUSE)
			{
				/* Save snake's direction and set pause*/
				snake->prev_direction = snake->direction;
				snake->direction = PAUSE;
				snake->showmode = SHOW_TEXT;
			}
			else
			{
				/* Retrieve former direction and run the snake */
				snake->direction = snake->prev_direction;
				snake->showmode = SHOW_WHOLE_SNAKE;
			}
		}
		/* Finally if characters is a valid direction - change direction (not allowed 180° changes)*/
		else
		{
			if ((snake->direction != PAUSE)
					&& !(snake->direction == LEFT && direction == RIGHT)
					&& !(snake->direction == RIGHT && direction == LEFT)
					&& !(snake->direction == UP && direction == DOWN)
					&& !(snake->direction == DOWN && direction == UP))
			{
				snake->direction = direction;
			}
		}
	}
}

/**
 * @brief  Function to draw snake border
 *
 * @note   Function draw 5 pixel thick border
 *
 * @param None
 * @retval None
 */
void platform_display_border(void)
{
	/* Small displays might not need*/
}

/**
 * @brief  Print text of size into upper-center,
 *
 * @note   Prints text with
 *
 *
 * @param str - string to print
 * @param length -
 * @param color -
 *
 * @retval None
 */
void platform_print_text_line_0(char *str, uint16_t length, uint16_t color)
{
	(void) (length);
	(void) (color);

	GLIB_drawStringOnLine(&glibContext, str, currentLine, GLIB_ALIGN_LEFT, 5, 5,
	true);
	DMD_updateDisplay();
}

/**
 * @brief  Print text of size into upper-center,
 *
 * @note   Prints text with
 *
 *
 * @param str - string to print
 * @param length -
 * @param color -
 *
 * @retval None
 */
void platform_print_text_line_1(char *str, uint16_t length, uint16_t color)
{
	(void) (length);
	(void) (color);

	GLIB_drawStringOnLine(&glibContext, str, currentLine+1, GLIB_ALIGN_LEFT, 5, 5,
	true);
	DMD_updateDisplay();
}

void platform_delay(uint32_t Delay, fn_t func)
{
	(void) (func);
	sl_sleeptimer_delay_millisecond(Delay);

}

void platform_save_score(uint32_t score)
{
	uint32_t prev_score = platform_load_score();
	score -= SNAKE_INIT_LNG;
	if (score > prev_score)
	{
		/* Add an SCORE_OBJ identifier */
		score |= SCORE_FLASH_OBJ;
		if (ECODE_OK
				!= nvm3_writeData(nvm3_defaultHandle, NVM3_KEY_MAX, &score,
						sizeof(uint32_t)))
		{
			platform_fatal();
		}
	}
}

uint32_t platform_load_score()
{
	uint32_t prev_score;

	if (ECODE_OK
			!= nvm3_readData(nvm3_defaultHandle, NVM3_KEY_MAX, &prev_score,
					sizeof(uint32_t)))
	{
		platform_fatal();
	}

	/* Check whether the score object is already present, to
	 * prevent from using an NVM inital random data value */
	if ((prev_score & 0xFF000000) != SCORE_FLASH_OBJ)
	{
		prev_score = 0;
	}
	else
	{
		prev_score &= 0x00FFFFFF;
	}

	return prev_score;
}
