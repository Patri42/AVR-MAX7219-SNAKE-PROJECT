#include "snake.h"
#include "snakegame.h"

void snake_init(Snake* snake) {
    snake->length = 1;
    snake->direction = RIGHT;
    snake->segments[0].position.x = 0;
    snake->segments[0].position.y = 0;
}

// Function to move the snake in its current direction
void snake_move(Snake* snake) {
    // First, we need to move each segment of the snake to the position of the segment in front of it.
    // Start from the tail and work up to avoid overwriting the positions we still need to copy.
    for (int i = snake->length - 1; i > 0; i--) {
        snake->segments[i].position = snake->segments[i - 1].position;
    }

    // Then, we move the head of the snake based on its current direction.
    switch (snake->direction) {
        case UP:
            snake->segments[0].position.y--;
            break;
        case DOWN:
            snake->segments[0].position.y++;
            break;
        case LEFT:
            snake->segments[0].position.x--;
            break;
        case RIGHT:
            snake->segments[0].position.x++;
            break;
    }
    
    // Wrap the snake around to the other side of the screen if it goes off the edge
    if (snake->segments[0].position.x < 0) snake->segments[0].position.x = MAX_X - 1;
    if (snake->segments[0].position.x >= MAX_X) snake->segments[0].position.x = 0;
    if (snake->segments[0].position.y < 0) snake->segments[0].position.y = MAX_Y - 1;
    if (snake->segments[0].position.y >= MAX_Y) snake->segments[0].position.y = 0;
}

// Function to change the snake's direction
void snake_turn(Snake* snake, Direction new_direction) {
    // We don't allow the snake to turn directly 180 degrees.
    if ((snake->direction == UP && new_direction != DOWN) ||
        (snake->direction == DOWN && new_direction != UP) ||
        (snake->direction == LEFT && new_direction != RIGHT) ||
        (snake->direction == RIGHT && new_direction != LEFT)) {
        snake->direction = new_direction;
    }
}

// Function to grow the snake by adding a new segment
void snake_grow(Snake* snake) {
    // Add a new segment to the snake at the end, effectively increasing its length by 1.
    // The new segment will be at the same position as the old last segment until the next move.
    if (snake->length < MAX_SNAKE_LENGTH) {
        snake->segments[snake->length] = snake->segments[snake->length - 1];
        snake->length++;
    }
}