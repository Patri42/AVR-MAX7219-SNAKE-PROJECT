#include "snakegame.h"
#include "stdbool.h"

Point food;

void place_food() {
    food.x = rand() % (MAX7219_SEG_NUM * 8); // Set food x-coordinate randomly
    food.y = rand() % (MAX7219_SEG_NUM * 8); // Set food y-coordinate randomly
}

void game_init() {
    place_food(); // Place the food in a random location at game start
}

bool is_food_eaten(Snake* snake) {
    if(snake->head->x == food.x && snake->head->y == food.y) {
        return true;
    }
    return false;
}

void update_game(Snake* snake) {
    if(is_food_eaten(snake)) {
        place_food(); // Place the food in a new location when it is eaten
        grow_snake(snake); // Grow the snake when food is eaten
    }
}

bool is_game_over(Game* game) {
    // Check if snake hits the boundaries of the screen
    if (game->snake->head->x < 0 || game->snake->head->x >= MAX7219_SEG_NUM * 8 ||
        game->snake->head->y < 0 || game->snake->head->y >= MAX7219_SEG_NUM * 8) {
        return true;
    }

    // Check if snake hits itself
    SnakeSegment* current = game->snake->head->next;
    while (current != NULL) {
        if (game->snake->head->x == current->x && game->snake->head->y == current->y) {
            return true;
        }
        current = current->next;
    }

    return false;
}