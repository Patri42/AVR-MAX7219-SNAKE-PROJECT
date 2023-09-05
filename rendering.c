#include <stdio.h>
#include "rendering.h"
#include "display.h"
#include "debug.h"
#include "millis.h"
#include "joystick.h"


// Define bitmaps for the letters 'G', 'A', 'M', 'E'
// These are 8x8 representations of the letters
const uint8_t LETTER_G[MAX7219_BUFFER_SIZE] = {0x3E, 0x41, 0x01, 0x79, 0x41, 0x41, 0x3E, 0x00};
const uint8_t LETTER_A[MAX7219_BUFFER_SIZE] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_M[MAX7219_BUFFER_SIZE] = {0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_E[MAX7219_BUFFER_SIZE] = {0x3F, 0x01, 0x01, 0x1F, 0x01, 0x01, 0x3F, 0x00};

void render_character(const uint8_t* character, uint8_t position) {
    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            if (character[row] & (1 << col)) {
                max7219b_set(col + position, row);
                // max7219b_set(row, col + position);
            }
        }
    }
}

void render_game_over_message() {
    DEBUG_PRINT("Entered render_game_over_message\n");
    uint8_t startPosition = MAX7219_BUFFER_SIZE; // Adjust this based on your display's size

	millis_t previousMillis = millis_get();
    millis_t currentMillis = 0; // initialize it with some value

    // Slide the "GAME" message from right to left
    //for (int position = 0; position < MAX7219_BUFFER_SIZE * 2; position++) {
    //for (int position = MAX7219_BUFFER_SIZE + 26; position >= -MAX7219_BUFFER_SIZE * 4; position--) {
    for (uint8_t position = MAX7219_BUFFER_SIZE + 26; position >= -MAX7219_BUFFER_SIZE; position--) {
        DEBUG_PRINT("Loop iteration started\n");
        // Clear the previous frame
        max7219b_clrAll();

        // Render each letter at the current position
        render_character(LETTER_E, position);
            DEBUG_PRINT("Rendering 'E' at position: %d\n", position);
        render_character(LETTER_M, position - 8);  // 8 columns away from G
            DEBUG_PRINT("Rendering 'M' at position: %d\n", position - 8);
        render_character(LETTER_A, position - 16); // 16 columns away from G
            DEBUG_PRINT("Rendering 'A' at position: %d\n", position - 16);
        render_character(LETTER_G, position - 24); // 24 columns away from G
            DEBUG_PRINT("Rendering 'G' at position: %d\n", position - 24);

        // Output the updated buffer to display
        max7219b_out();

        // Non-blocking delay using millis()
        while(true) {
            currentMillis = millis_get();
            
            if(currentMillis - previousMillis >= 50) { // Check if 50ms has passed
                previousMillis = currentMillis; // Reset the previous time
                break;
            }
            DEBUG_PRINT("Checking select button\n");
            if(digitalRead(SEL_PIN) == LOW) {  // If the select button is pressed
                DEBUG_PRINT("Select button pressed, exiting render_game_over_message\n");
                return;  // Exit the function
            } 
        }
         
        DEBUG_PRINT("Loop iteration ended\n"); 
    }
    DEBUG_PRINT("Exiting render_game_over_message\n");
}

void render_game(Game* game) {
    // Clear the previous frame
    max7219b_clrAll();

    // Render snake
    for(uint8_t i = 0; i < game->snake.length; i++) {
        uint8_t x = game->snake.segments[i].x;
        uint8_t y = game->snake.segments[i].y;
        if (x >= 0 && y >= 0) { // Only render if coordinates are valid
            DEBUG_PRINT("Rendering snake segment at (%d, %d)\n", x, y); // Debug Print 
            max7219b_set(y, x);
            
        }
    }

    // Render the food
    uint8_t foodX = game->food.x;
    uint8_t foodY = game->food.y;
    DEBUG_PRINT("Rendering food segment at (%d, %d)\n", foodX, foodY); // Debug Print 
    // max7219b_set(foodY, foodX);
    max7219b_set(foodX, foodY);

    // Output the updated buffer to display
    max7219b_out();
}