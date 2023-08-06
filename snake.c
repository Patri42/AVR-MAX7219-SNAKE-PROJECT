#include "snake.h"

void snake_init(Snake* snake) {
    snake->length = 1;
    snake->direction = RIGHT;
    snake->segments[0].position.x = 0;
    snake->segments[0].position.y = 0;
}

void snake_move(Snake* snake) {
    // code to move the snake in its current direction
}

void snake_turn(Snake* snake, Direction new_direction) {
    // code to change the snake's direction
}

void snake_grow(Snake* snake) {
    // code to grow the snake by adding a new segment
}