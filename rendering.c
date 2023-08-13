#include <stdio.h>
#include "rendering.h"
#include "display.h"
#include "debug.h"
#include "millis.h"


// Define bitmaps for the letters 'G', 'A', 'M', 'E'
// These are 8x8 representations of the letters
const uint8_t LETTER_G[MAX7219_BUFFER_SIZE] = {0x3C, 0x42, 0x42, 0x42, 0x32, 0x02, 0x3C, 0x00};
const uint8_t LETTER_A[MAX7219_BUFFER_SIZE] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_M[MAX7219_BUFFER_SIZE] = {0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_E[MAX7219_BUFFER_SIZE] = {0x7E, 0x02, 0x02, 0x3E, 0x02, 0x02, 0x7E, 0x00};

// uint8_t letter_G[MATRIX_WIDTH] = {
//     0b00111100,
//     0b01000000,
//     0b01000110,
//     0b01000110,
//     0b01000010,
//     0b01000010,
//     0b00111100,
//     0b00000000
// };

// Function to render a single 8x8 character at a given position
void render_character(const uint8_t* character, int position) {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (character[row] & (1 << col)) {
                // Set the pixel at the current position
                // You'll need to adapt this line to match your rendering code
                max7219b_set(row, col + position);
            }
        }
    }
}

void render_game_over_message() {
    int startPosition = MAX7219_BUFFER_SIZE; // Adjust this based on your display's size

    // Slide the "GAME" message from right to left
    for (int position = startPosition; position > -MAX7219_BUFFER_SIZE * 4; position--) {
        // Clear the previous frame
        max7219b_clrAll();

        // Render each letter at the current position
        render_character(LETTER_G, position);
        render_character(LETTER_A, position + MAX7219_BUFFER_SIZE);
        render_character(LETTER_M, position + MAX7219_BUFFER_SIZE * 2);
        render_character(LETTER_E, position + MAX7219_BUFFER_SIZE * 3);

        // Output the updated buffer to display
        max7219b_out();

        // Non-blocking delay using millis()
        while(true) {
            currentMillis = millis_get();
            if(currentMillis - previousMillis >= 100) { // Check if 100ms has passed
                previousMillis = currentMillis; // Reset the previous time
                break;
			}
    	}
	}
}

void render_game(Game* game) {
    // Clear the previous frame
    max7219b_clrAll();

    // Render snake
    for(int i = 0; i < game->snake.length; i++) {
        int x = game->snake.segments[i].x;
        int y = game->snake.segments[i].y;
        if (x >= 0 && y >= 0) { // Only render if coordinates are valid
            DEBUG_PRINT("Rendering snake segment at (%d, %d)\n", x, y); // Debug Print 
            max7219b_set(y, x);
        }
    }

    // Render the food
    int foodX = game->food.x;
    int foodY = game->food.y;
    DEBUG_PRINT("Rendering food segment at (%d, %d)\n", foodX, foodY); // Debug Print 
    max7219b_set(foodY, foodX);

    // Output the updated buffer to display
    max7219b_out();
}