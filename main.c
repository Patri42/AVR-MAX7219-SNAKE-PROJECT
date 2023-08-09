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

// Wokwi:
// https://wokwi.com/projects/296234816685212169

#include <avr/interrupt.h>
#include <avr/io.h>
#include "joystick.h"
#include "display.h"
#include <stdlib.h> 
#include <util/delay.h>
#include <stdbool.h>
#include <time.h>
#include "millis.h"
#include "uart.h"

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

// Initialize the snake
void snake_init(Snake* snake) {
    snake->length = 1;
    for(int i = 0; i < MAX_SNAKE_LENGTH; i++) {
        snake->segments[i].x = (i == 0) ? 1 : -1; // Only the head is given a valid position
        snake->segments[i].y = (i == 0) ? 1 : -1;
    }
}

// Move the snake
void snake_move(Snake* snake, int dirX, int dirY) {
    // Save the tail segment's position
    Segment tail = snake->segments[snake->length - 1];
    
    // Move the tail
    for(int i = snake->length - 1; i > 0; i--) {
        snake->segments[i] = snake->segments[i - 1];
    }

    // Move the head
    snake->segments[0].x += dirX;
    snake->segments[0].y += dirY;

    // If the snake has grown, put the saved tail segment back at the end
    if (snake->hasGrown) {
        snake->segments[snake->length - 1] = tail;
        snake->hasGrown = false; // Reset the growth flag
    }
}

// Grow the snake
void snake_grow(Snake* snake) {
    if(snake->length < MAX_SNAKE_LENGTH) {
        snake->length++;
        snake->hasGrown = true; // Set a flag indicating that the snake has grown
    }
}

// Check collision with food
bool check_food_collision(Snake* snake, int foodX, int foodY) {
    return snake->segments[0].x == foodX && snake->segments[0].y == foodY;
}


void render_game(Game* game) {
    // Clear the previous frame
    max7219b_clrAll();

    // Render snake
    for(int i = 0; i < game->snake.length; i++) {
        int x = game->snake.segments[i].x;
        int y = game->snake.segments[i].y;
        if (x >= 0 && y >= 0) { // Only render if coordinates are valid
            printf("Rendering snake segment at (%d, %d)\n", x, y); // Debug Print 
            max7219b_set(y, x);
        }
    }

    // Render the food
    int foodX = game->food.x;
    int foodY = game->food.y;
    printf("Rendering food segment at (%d, %d)\n", foodX, foodY); // Debug Print 
    max7219b_set(foodY, foodX);

    // Output the updated buffer to display
    max7219b_out();
}

Direction read_joystick_direction(Direction current_direction) {
    int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
    int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

    if (vert < JOYSTICK_THRESHOLD_LOW) return UP;
    if (vert > JOYSTICK_THRESHOLD_HIGH) return DOWN;
    if (horz < JOYSTICK_THRESHOLD_LOW) return RIGHT;
    if (horz > JOYSTICK_THRESHOLD_HIGH) return LEFT;

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
    printf("Init food placement\n");
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
        printf("Game over due to boundary collision\n"); // Debugging print statement
        return true;
    }

    // Check if the snake hits itself
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.segments[0].x == game->snake.segments[i].x &&
            game->snake.segments[0].y == game->snake.segments[i].y) {
            printf("Game over due to self collision\n"); // Debugging print statement
            return true;
        }
    }

    return false;
}

void game_loop() {
    // Initialize the millisecond tracking
    millis_init();
    //sei();
    printf("Millis init\n");


    // Initialize the game state
    Game game;
    init_game(&game);
    printf("Game init\n");

    // Initial direction
    Direction current_direction = RIGHT;

    // Set up time tracking
    millis_t lastUpdateTime = millis_get();
    const millis_t updateInterval = 500; // Update every 500 milliseconds
    printf("Millis setup\n");

    while (true) {
        // Check the time since the last update
        millis_t currentTime = millis_get();
        //printf("Current time: %lu\n", currentTime);
        if (currentTime - lastUpdateTime >= updateInterval) {
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
                printf("New food init\n");
                snake_grow(&game.snake);
                printf("New food position (%d, %d)\n", game.food.x, game.food.y); // Debugging print statement
                printf("Snake head position (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y); // Debugging print statement
            }

            // Render the snake on the display
            render_game(&game);

            // Check for game over condition
            if (is_game_over(&game)) {
                printf("Game over detected\n"); // Debugging print statement
                printf("Snake head position at game over (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y); // Debugging print statement
                // Handle the game over (e.g., display a message, reset the game, etc.)
                break;
            }

            // Record the time of this update
            lastUpdateTime = currentTime;
        }

        // You can add any other non-blocking tasks here if necessary
        // ...
    }
}

int main(void) {
    srand(time(NULL));
    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize the display (if applicable)
    adc_init(); // Initialize ADC for joystick

    millis_init(); // Initialize millis
    sei(); // Enable global interrupts

    game_loop();     // Start the game loop
    return 0;        // Return code
}