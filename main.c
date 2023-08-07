/**
 * Hardware Setup
 * 
 * Joystick:
 * - HORZ is connected to Arduino Uno pin A1
 * - VERT is connected to Arduino Uno pin A0
 * - SEL is connected to Arduino Uno pin D2
 * - GND is connected to Arduino Uno pin GND
 * - VCC is connected to Arduino Uno pin 5V
 * 
 * Matrix:
 * - CS is connected to Arduino Uno pin D10
 * - DIN is connected to Arduino Uno pin D11
 * - CLK is connected to Arduino Uno pin D13
 * - GND is connected to Arduino Uno pin GND
 * - VCC is connected to Arduino Uno pin 5V
 */

#include "joystick.h"
#include "display.h"
#include <stdlib.h> 
#include <util/delay.h>
#include <stdbool.h>
#include <time.h>

#define MAX_SNAKE_LENGTH 100

typedef struct {
    int x, y;
} Segment;

typedef struct {
    Segment segments[MAX_SNAKE_LENGTH];
    int length;
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

// Initialize the snake
void snake_init(Snake* snake) {
    snake->length = 1;
    for(int i = 0; i < snake->length; i++) {
        snake->segments[i].x = 1 - i; // Starting position
        snake->segments[i].y = 1;
    }
}

// Move the snake
void snake_move(Snake* snake, int dirX, int dirY) {
    // Move the tail
    for(int i = snake->length - 1; i > 0; i--) {
        snake->segments[i] = snake->segments[i - 1];
    }
    // Move the head
    snake->segments[0].x += dirX;
    snake->segments[0].y += dirY;
}

// Grow the snake
// void snake_grow(Snake* snake) {
//     if(snake->length < MAX_SNAKE_LENGTH) {
//         snake->segments[snake->length] = snake->segments[snake->length - 1];
//         snake->length++;
//         printf("Snake grown to length: %d\n", snake->length);
//     } else {
//         printf("Snake at max length\n");
//     }
// }

void snake_grow(Snake* snake) {
    if(snake->length < MAX_SNAKE_LENGTH) {
        // Add the new segment at the tail of the snake, without changing its position
        snake->length++;
    }
}

// Check collision with food
bool check_food_collision(Snake* snake, int foodX, int foodY) {
    return snake->segments[0].x == foodX && snake->segments[0].y == foodY;
}

void render_snake(Snake* snake) {
    // Clear the previous frame
    max7219b_clrAll();

    // Set the pixels for the snake's segments
    for(int i = 0; i < snake->length; i++) {
        int x = snake->segments[i].x;
        int y = snake->segments[i].y;
        max7219b_set(y, x);
    }

    // Output the updated buffer to display
    max7219b_out();
}

void render_game(Game* game) {
    // Clear the previous frame
    max7219b_clrAll();

    // Render snake
    for(int i = 0; i < game->snake.length; i++) {
        int x = game->snake.segments[i].x;
        int y = game->snake.segments[i].y;
        max7219b_set(y, x);
    }

    // Render the food
    int foodX = game->food.x;
    int foodY = game->food.y;
    max7219b_set(foodY, foodX);

    // Output the updated buffer to display
    max7219b_out();
}

Direction read_joystick_direction(Direction current_direction) {
    int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
    int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

    if (vert < 300) return UP;
    if (vert > 700) return DOWN;
    if (horz < 300) return RIGHT;
    if (horz > 700) return LEFT;

    return current_direction; // Keep the current direction if no change
}

bool is_opposite_direction(Direction current, Direction new_direction) {
    if (current == UP && new_direction == DOWN) return true;
    if (current == DOWN && new_direction == UP) return true;
    if (current == LEFT && new_direction == RIGHT) return true;
    if (current == RIGHT && new_direction == LEFT) return true;
    return false;
}

int x = 0; // Initialize x position
int y = 0; // Initialize y position
Direction current_direction = RIGHT; // Start with a right direction

void init_game(Game* game) {
    snake_init(&(game->snake)); // Initialize the snake
    place_food(game); // Place the food in a random location at game start
    game->isGameOver = false; // Set game over flag to false
}

void update_snake_direction(Snake* snake, Direction current_direction) {
    int dirX = 0;
    int dirY = 0;
    switch (current_direction) {
        case UP: dirY = -1; break;
        case DOWN: dirY = 1; break;
        case LEFT: dirX = -1; break;
        case RIGHT: dirX = 1; break;
    }
    snake_move(snake, dirX, dirY);
}

void place_food(Game* game) {
    bool collision;
    do {
        collision = false;
        game->food.x = rand() % (MAX7219_SEG_NUM * 8);
        game->food.y = rand() % (MAX7219_SEG_NUM * 8);
        for(int i = 0; i < game->snake.length; i++) {
            if(game->snake.segments[i].x == game->food.x && game->snake.segments[i].y == game->food.y) {
                collision = true;
                break;
            }
        }
    } while (collision); // Repeat if food is placed on top of the snake
}

bool is_game_over(Game* game) {
    // Check if the snake hits the boundaries of the screen
    if (game->snake.segments[0].x < 0 || game->snake.segments[0].x >= (MAX7219_SEG_NUM * 8) ||
        game->snake.segments[0].y < 0 || game->snake.segments[0].y >= (MAX7219_SEG_NUM * 8)) {
        printf("Game over due to boundary collision\n");
        return true;
    }

    // Check if the snake hits itself
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.segments[0].x == game->snake.segments[i].x &&
            game->snake.segments[0].y == game->snake.segments[i].y) {
            printf("Game over due to self collision\n");
            return true;
        }
    }

    return false;
}

void game_loop() {
    // Initialize the game state
    Game game;
    init_game(&game);

    // Initial direction
    Direction current_direction = RIGHT;

    while (true) {
        // Read the input from the joystick (or other input method)
        Direction new_direction = read_joystick_direction(current_direction);

        // Update the direction if it's not opposite to the current one
        if (!is_opposite_direction(current_direction, new_direction)) {
            current_direction = new_direction;
        }

        // Move the snake in the current direction
        update_snake_direction(&game.snake, current_direction);

        // Check for food collision and grow the snake if necessary
        if (check_food_collision(&game.snake, game.food.x, game.food.y)) {
            printf("Food collision detected\n"); // Debugging print statement
            place_food(&game);
            snake_grow(&game.snake);
            printf("New food position (%d, %d)\n", game.food.x, game.food.y);
            printf("Snake head position (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y);
        }

        // Render the snake on the display
        render_game(&game);

        // Check for game over condition
        if (is_game_over(&game)) {
            printf("Game over detected\n"); // Debugging print statement
            printf("Snake head position at game over (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y);
            // Handle the game over (e.g., display a message, reset the game, etc.)
            break;
        }

        // Delay to control the speed of the game
        _delay_ms(500);
    }
}

int main(void) {
    srand(time(NULL));
    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize the display (if applicable)
    adc_init(); // Initialize ADC for joystick


    game_loop();     // Start the game loop
    return 0;        // Return code
}