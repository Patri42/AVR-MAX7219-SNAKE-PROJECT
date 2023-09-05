
#include <stdlib.h>
#include "game_logic.h"
#include "joystick.h"
#include "millis.h"
#include "debug.h"
#include <util/delay.h>

// Initialize the snake
void snake_init(Snake* snake) {
    snake->length = 1;
    //Randomize the starting position of the head within the game's boundaries
    snake->segments[0].x = rand() % (MAX7219_SEG_NUM * 8);
    snake->segments[0].y = rand() % (MAX7219_SEG_NUM * 8);
    

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
bool check_food_collision(Snake* snake, int foodY, int foodX) {
    return snake->segments[0].x == foodX && snake->segments[0].y == foodY;
    
}

Direction read_joystick_direction(Direction current_direction) {
    int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
    int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

    if (vert < JOYSTICK_THRESHOLD_LOW) return LEFT;
    if (vert > JOYSTICK_THRESHOLD_HIGH) return RIGHT;
    if (horz < JOYSTICK_THRESHOLD_LOW) return UP;
    if (horz > JOYSTICK_THRESHOLD_HIGH) return DOWN;

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

void place_food(Game* game) {
    DEBUG_PRINT("Placing food\n"); // Debugging
    bool collision;
    do {
        collision = false;
        game->food.x = rand() % (MAX7219_SEG_NUM * 8);
        game->food.y = rand() % 8;
        for(int i = 0; i < game->snake.length; i++) {
            if(game->snake.segments[i].x == game->food.x && game->snake.segments[i].y == game->food.y) {
                collision = true;
                break;
            }
        }
    } while (collision); // Repeat if food is placed on top of the snake
    DEBUG_PRINT("New food position (%d, %d)\n", game->food.x, game->food.y); // Debugging
}

void init_game(Game* game) {
    snake_init(&(game->snake)); // Initialize the snake
    place_food(game); // Place the food in a random location at game start
    DEBUG_PRINT("Init food placement\n");
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

bool is_game_over(Game* game) {
    // Check if the snake hits the boundaries of the screen
    // if (game->snake.segments[0].x < 0 || game->snake.segments[0].x >= (MAX7219_SEG_NUM * 8) ||
    //     // game->snake.segments[0].y < 0 || game->snake.segments[0].y >= (MAX7219_SEG_NUM * 8)) {
    //     //game->snake.segments[0].y < 0 || game->snake.segments[0].y >= 8) {
    //     game->snake.segments[0].y < 0 || game->snake.segments[0].y >= (MAX7219_SEG_NUM * 8)) {
    //     DEBUG_PRINT("Game over due to boundary collision\n"); // Debugging print statement
    //     return true;
    // }
    if (game->snake.segments[0].x < 0 || game->snake.segments[0].x >= (8) ||
        game->snake.segments[0].y < 0 || game->snake.segments[0].y >= (MAX7219_SEG_NUM * 8)) {
        DEBUG_PRINT("Game over due to boundary collision\n"); // Debugging print statement
        return true;
    }

    // Check if the snake hits itself
    for (int i = 1; i < game->snake.length; i++) {
        if (game->snake.segments[0].x == game->snake.segments[i].x &&
            game->snake.segments[0].y == game->snake.segments[i].y) {
            DEBUG_PRINT("Game over due to self collision\n"); // Debugging print statement
            return true;
        }
    }

    return false;
}

void game_loop() {
    // Initialize the millisecond tracking
    millis_init();
    //sei();
    DEBUG_PRINT("Millis init\n");


    // Initialize the game state
    Game game;
    init_game(&game);
    DEBUG_PRINT("Game init\n");

    // Initial direction
    Direction current_direction = RIGHT;

    // Set up time tracking
    millis_t lastUpdateTime = millis_get();
    const millis_t updateInterval = 300; // Update every 500 milliseconds
    DEBUG_PRINT("Millis setup\n");

    while (true) {
        // Check the time since the last update
        millis_t currentTime = millis_get();
        //DEBUG_PRINT("Current time: %lu\n", currentTime);
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
                DEBUG_PRINT("Food collision detected\n"); // Debugging print statement
                //update_snake_direction(&game.snake, current_direction);
                snake_grow(&game.snake);  // <-- Move snake_grow() here
                //update_snake_direction(&game.snake, current_direction); 
                place_food(&game);
                DEBUG_PRINT("New food init\n");
                // snake_grow(&game.snake);
                DEBUG_PRINT("New food position (%d, %d)\n", game.food.x, game.food.y); // Debugging print statement
                DEBUG_PRINT("Snake head position (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y); // Debugging print statement
            }

            // Render the snake on the display
            render_game(&game);

            // Check for game over condition
            if (is_game_over(&game)) {
                DEBUG_PRINT("Game over detected\n"); // Debugging print statement
                DEBUG_PRINT("Snake head position at game over (%d, %d)\n", game.snake.segments[0].x, game.snake.segments[0].y); // Debugging print statement
                // Handle the game over (e.g., display a message, reset the game, etc.)
                // Handle the game over (display the "GAME" message)
                render_game_over_message();

                wait_for_select_button();

                //_delay_ms(3000); // Wait for 3 seconds

                init_game(&game); // Re-initialize the game to start over
                current_direction = RIGHT; // Reset the initial direction or any other states if necessary
                lastUpdateTime = millis_get(); // Reset the update time
                
                //break;
            }

            // Record the time of this update
            lastUpdateTime = currentTime;
        }   
    }
}

