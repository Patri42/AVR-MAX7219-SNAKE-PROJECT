#include "display.h"


void display_init(void) {
    init_serial();
    max7219_init();
}

// Sets a specific pixel on the display at position (x, y).
void display_set(uint8_t x, uint8_t y) {
    max7219b_set(y, x);
}

// Outputs the current state of the MAX7219 buffer to the display.
void display_output(void) {
    max7219b_out();
}

// Function to clear max7219 buffer
void max7219b_clrAll(void)
{
    for (uint8_t i = 0; i < MAX7219_BUFFER_SIZE; i++) {
        max7219_buffer[i] = 0;
    }
}
