#ifndef GAME_H
#define GAME_H

#include "snake.h"

typedef struct {
    int x;
    int y;
} Point;

extern Point food;

void game_init(void);
void place_food(void);
bool is_food_eaten(Snake* snake);
void update_game(Snake* snake);

#endif // GAME_H