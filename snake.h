#ifndef SNAKE_H
#define SNAKE_H

#define MAX_SNAKE_LENGTH 64

// Enum for the direction the snake is currently moving in
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

// Structure for a point on the screen
typedef struct {
    int x;
    int y;
} Point;

// Definition for one segment of the snake
typedef struct {
    Point position;
} Segment;

// Definition for the whole snake
typedef struct {
    Segment segments[MAX_SNAKE_LENGTH];
    int length;
    Direction direction;
} Snake;

void snake_init(Snake* snake);
void snake_move(Snake* snake);
void snake_turn(Snake* snake, Direction new_direction);
void snake_grow(Snake* snake);

#endif // SNAKE_H