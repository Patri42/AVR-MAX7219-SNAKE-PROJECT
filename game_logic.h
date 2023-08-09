#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>

#define MAX_SNAKE_LENGTH 100


typedef struct {
    int x, y;
} Segment;

typedef struct {
    Segment segments[MAX_SNAKE_LENGTH];
    int length;
    bool hasGrown;
} Snake;

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef struct {
    int x, y;
} Point;

typedef struct {
    Snake snake; // The snake object
    Point food;  // The food's position
    bool isGameOver; // Flag for game over status
} Game;

void snake_init(Snake* snake);
void snake_move(Snake* snake, int dirX, int dirY);
void snake_grow(Snake* snake);
bool check_food_collision(Snake* snake, int foodX, int foodY);
Direction read_joystick_direction(Direction current_direction);
bool is_opposite_direction(Direction current, Direction new_direction);
void init_game(Game* game);
void update_snake_direction(Snake* snake, Direction current_direction);
void place_food(Game* game);
bool is_game_over(Game* game);
void game_loop();

#endif