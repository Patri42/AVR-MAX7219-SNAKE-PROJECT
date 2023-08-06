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

// Setup: https://wokwi.com/projects/296234816685212169

#include "joystick.h"
#include "display.h"
#include <util/delay.h>
#include "snakegame.h"
#include "snake.h"

int main () {
    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize max7219 display
    adc_init(); // Initialize ADC for joystick

    
    // Set a single dot on the matrix to on - TEST
    max7219b_set(2, 3); 
    
    pinMode(VERT_PIN, INPUT); // Set vertical joystick pin as input
    pinMode(HORZ_PIN, INPUT); // Set horizontal joystick pin as input
    pinMode(SEL_PIN, INPUT_PULLUP); // Set selection button pin as input with pull-up

    // Initialize game
    Game game;
    init_game(&game);

    while(1) {
        int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
        int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

        // Update game based on joystick input
        if (vert < 300) {
            update_game(&(game.snake), UP);
        }
        if (vert > 700) {
            update_game(&(game.snake), DOWN);
        }
        if (horz > 700) {
            update_game(&(game.snake), RIGHT);
        }
        if (horz < 300) {
            update_game(&(game.snake), LEFT);
        }

        // Handle game events
        if (is_food_eaten(&(game.snake))) {
            place_food();
            snake_grow(&(game.snake));
        }

        // Check if game is over
        if (is_game_over(&game)) {
            // Display "Game Over" message or do something else here
            break;
        }


        // Draw game state
        max7219b_clrAll(); // Clear the display
        //draw_game(&game); // Draw the game state
        max7219b_out(); // Output the updated buffer to display

        _delay_ms(100); // Delay for 100 ms for debouncing
    }

    return 0;
}