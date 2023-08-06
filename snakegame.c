#include "snakegame.h"
#include "stdbool.h"
#include "max72xx.h"
#include "snake.h"

Point food;

void place_food() {
    food.x = rand() % (MAX7219_SEG_NUM * 8); // Set food x-coordinate randomly
    food.y = rand() % (MAX7219_SEG_NUM * 8); // Set food y-coordinate randomly
}

void init_game(Game* game) {
    // Initialize the snake
    snake_init(&(game->snake));

    // Place the food in a random location at game start
    place_food();
}

bool is_food_eaten(Snake* snake) {
    if(snake->segments[0].position.x == food.x && snake->segments[0].position.y == food.y) {
        return true;
    }
    return false;
}

void update_game(Snake* snake, Direction new_direction) {
    update_snake_direction(snake, new_direction);
    if(is_food_eaten(snake)) {
        place_food(); // Place the food in a new location when it is eaten
        snake_grow(snake); // Grow the snake when food is eaten
    }
}

void update_snake_direction(Snake* snake, Direction new_direction) {
    snake_turn(snake, new_direction);
}

bool is_game_over(Game* game) {
    // Check if snake hits the boundaries of the screen
    if (game->snake.segments[0].position.x < 0 || game->snake.segments[0].position.x >= MAX7219_SEG_NUM * 8 ||
        game->snake.segments[0].position.y < 0 || game->snake.segments[0].position.y >= MAX7219_SEG_NUM * 8) {
        return true;
    }

    // Check if snake hits itself
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.segments[0].position.x == game->snake.segments[i].position.x && 
            game->snake.segments[0].position.y == game->snake.segments[i].position.y) {
            return true;
        }
    }

    return false;
}