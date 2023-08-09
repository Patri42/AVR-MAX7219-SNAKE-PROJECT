#include "rendering.h"
#include "display.h"
#include "debug.h"


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