#include "display.h"

//uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];


void display_init(void) {
    init_serial();
    max7219_init();
}

void display_clear(void) {
    for (uint8_t i = 0; i < MAX7219_BUFFER_SIZE; i++) {
        max7219_buffer[i] = 0;
    }
}

void display_set(int x, int y) {
    max7219b_set(y, x);
}

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
