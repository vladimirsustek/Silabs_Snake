/*
 * snake_game.c
 *
 *  Created on: Oct 1, 2023
 *      Author: vlasus
 */
#include "snake_game.h"

snake_game_t* snake_game_init()
{
	static snake_game_t game;

	snake_init(&game.snake, &game.food, &game.progCycle);

	return &game;

}

bool snake_game_cycle(snake_game_t *game)
{
	snake_control(&game->snake);
	snake_move(&game->snake);
	snake_inform(&game->snake, &game->food);

	if (game->snake.state == GAME_OVER || game->snake.state == GAME_WON)
	{
		if (game->snake.direction != PAUSE)
		{
			return true;
		}
		else
		{
			snake_save_score(game->snake.length);
			return false;
		}
	}

	snake_haseaten(&game->snake, &game->food);
	snake_display(&game->snake, &game->food);
	snake_place_food(&game->snake, &game->food, &game->progCycle);

	return true;
}
