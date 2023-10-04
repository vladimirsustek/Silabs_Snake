/*
 * Porting layer for a snake game (snake_functions)
 *
 * port.h
 *
 *  Created on: Feb 27, 2022
 *      Author: Vladimir Sustek
 *      https://github.com/vladimirsustek
 */
#ifndef SNAKE_PORT_H_
#define SNAKE_PORT_H_

#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* Platform - LCD dependencies */
/* Platform MCU randomizer dependencies */

/* Control dependencies */

/* Hardware dependent constants - LCD TFT 3.5'' 320x480
 *
 * all these macros must be adjusted to a concrete display:
 *
 * ARENA_OFFSET_X, ARENA_OFFSET_Y
 * CELL_SIZE
 * ARENA_MAX_X, ARENA_MAX_Y
 * FOOD_MAX_X, FOOD_MAX_Y
 *
 *    0  1~~13 14
 *   ------------
 * 0 |
 * 1 |
 * ~~     ARENA
 * 20|
 * 21|
 *   ------------ height = 480 pixels
 *                width = 320 pixels
 *
 *  Mathematically proof:
 *  ARENA_OFFSET_X + ARENA_MAX_X*CELL_SIZE + ARENA_OFFSET_X = DISPLAY_WIDTH
 *  6 + 14*22 + 6 = 320 (pixels)
 *  ARENA_OFFSET_Y + ARENA_MAX_Y*CELL_SIZE + ARENA_OFFSET_Y = DISPLAY_HEIGHT
 *  6 + 14*22 + 6 = 320 (pixels)
 */

/* Offset of used are by game on X in pixels */
#define ARENA_OFFSET_X		(uint16_t)(4)
/* Offset of used are by game on Y in pixels */
#define ARENA_OFFSET_Y		(uint16_t)(4)

/* Size of the cell in pixels */
#define CELL_SIZE			(uint16_t)(8)

/* Maximal coordination X and Y axis for a cell */
#define ARENA_MAX_X			(uint16_t)(16)
#define ARENA_MAX_Y			(uint16_t)(16)
#define ARENA_MIN_X			(uint16_t)(0)
#define ARENA_MIN_Y			(uint16_t)(0)

/* Maximal* coordination X and Y for a food cell */
#define FOOD_MAX_X			(uint16_t)(15)
#define FOOD_MIN_X			(uint16_t)(1)
#define FOOD_MAX_Y			(uint16_t)(15)
#define FOOD_MIN_Y			(uint16_t)(1)

/* General constants (applicable across platforms) */
#define SNAKE_MAX_LNG		(uint16_t)(250)
#define SNAKE_WON_LIMIT		(uint16_t)(SNAKE_MAX_LNG - 1)

/* Definition, where snake starts and how is long */
#define SNAKE_INIT_X_CORD	(uint16_t)(1)
#define SNAKE_INIT_Y_CORD	(uint16_t)(10)
#define SNAKE_INIT_LNG		(uint16_t)(3)

/* Return used for snake functions */
#define SNAKE_MOVE_OK		(uint16_t)(0)
#define SNAKE_MOVE_CRASH	(uint16_t)(1)
#define SNAKE_WON			(uint16_t)(2)
#define GENERAL_ERROR		(uint16_t)(-1)
#define INVALID_COORDS		(uint16_t)(-1)

/* Number of maximal iterations for randomizer */
#define FOOD_MAX_ITER		(uint16_t)(10)

typedef enum
{
	UP = 'W', DOWN = 'S', LEFT = 'A', RIGHT = 'D', PAUSE = 'P', QUIT = 'Q'
} snake_dir_e;

typedef enum
{
	FOOD_WAITING, FOOD_PLACED, FOOD_EATEN
} foodstate_e;

typedef enum
{
	GAME_ONGOING, GAME_OVER, GAME_WON
} snake_state_e;

typedef enum
{
	SHOW_TEXT, SHOW_WHOLE_SNAKE, SHOW_SNAKE_MOVE
} snake_infor_e;

typedef struct coord_tag
{
	uint16_t x;
	uint16_t y;
} coord_t;
;
typedef struct snake_tag
{
	snake_dir_e direction;
	snake_dir_e prev_direction;
	coord_t body[SNAKE_MAX_LNG ];
	uint16_t length;
	coord_t ghost;
	snake_infor_e showmode;
	snake_state_e state;
} snake_t;

typedef struct food_tag
{
	coord_t coord;
	foodstate_e state;
	uint16_t time_elapsed;
	uint8_t rePrintFood;
} food_t;

/* pointer to function returning uint32_t with uint32_t parameter
 * used for snake_delay as an function called during blocking delay */
typedef uint32_t
fn_t(uint32_t);

void
platform_drawCell(uint16_t x, uint16_t y);
void
platform_eraseCell(uint16_t x, uint16_t y);
void
platform_drawFood(uint16_t x, uint16_t y);
void
platform_eraseFood(uint16_t x, uint16_t y);

void
platform_init(void);
void
platform_init_randomizer(void);
void
platform_printXY(uint16_t x, uint16_t y, char symbol);
uint16_t
platform_randomize(void);
uint16_t
platform_msTickGet(void);
void
platform_fatal(void);
void
platform_get_control(snake_t *snake);
void
platform_refresh_hw(void);
void
platform_display_border(void);
void platform_print_text_line_0(char *str, uint16_t length, uint16_t color);
void platform_print_text_line_1(char *str, uint16_t length, uint16_t color);
void platform_snake_set_control(char c);
void
platform_delay(uint32_t Delay, fn_t func);

void platform_save_score(uint32_t score);

uint32_t platform_load_score();

#endif /* SNAKE_PORT_H_ */
