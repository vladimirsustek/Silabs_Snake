/*
 * snake_game.h
 *
 *  Created on: Oct 1, 2023
 *      Author: vlasus
 */

#ifndef SNAKEGAME_SNAKE_GAME_H_
#define SNAKEGAME_SNAKE_GAME_H_

#include "snake_function.h"
#include <stdbool.h>

typedef struct
{
	snake_t snake;
	food_t food;
	uint32_t progCycle;
} snake_game_t;

snake_game_t*
snake_game_init();
bool
snake_game_cycle(snake_game_t *game);

#endif /* SNAKEGAME_SNAKE_GAME_H_ */
