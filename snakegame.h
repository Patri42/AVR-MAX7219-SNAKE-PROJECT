#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include "stdbool.h"
#include "snake.h"

#define MAX_X 8
#define MAX_Y 8

typedef struct {
    Snake snake;           // The snake object
    int foodX;              // X coordinate of the food
    int foodY;              // Y coordinate of the food
    int score;              // Current score
    bool isGameOver;        // Flag to check if the game is over
} Game;

typedef struct {
    int x;
    int y;
} Point;

extern Point food;

void game_init(void);
void place_food(void);
bool is_food_eaten(Snake* snake);
void update_game(Snake* snake, Direction new_direction);

#endif 