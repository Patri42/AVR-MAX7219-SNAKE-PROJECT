#include "rendering.h"
#include "display.h"
#include "debug.h"

#define LETTER_WIDTH 8

// Define bitmaps for the letters 'G', 'A', 'M', 'E'
// These are 8x8 representations of the letters
const uint8_t LETTER_G[8] = {0x3C, 0x42, 0x42, 0x42, 0x32, 0x02, 0x3C, 0x00};
const uint8_t LETTER_A[8] = {0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_M[8] = {0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00};
const uint8_t LETTER_E[8] = {0x7E, 0x02, 0x02, 0x3E, 0x02, 0x02, 0x7E, 0x00};

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
    for (int position = startPosition; position > -LETTER_WIDTH * 4; position--) {
        // Clear the previous frame
        max7219b_clrAll();

        // Render each letter at the current position
        render_character(LETTER_G, position);
        render_character(LETTER_A, position + LETTER_WIDTH);
        render_character(LETTER_M, position + LETTER_WIDTH * 2);
        render_character(LETTER_E, position + LETTER_WIDTH * 3);

        // Output the updated buffer to display
        max7219b_out();

        // Delay for a short time to create the sliding effect
        _delay_ms(100);
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