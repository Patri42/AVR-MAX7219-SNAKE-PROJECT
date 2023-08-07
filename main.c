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

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

int main () {
    int x = 0; // Initialize x position
    int y = 0; // Initialize y position

    int foodX = rand() % (MAX7219_SEG_NUM * 8); // Food x-coordinate
    int foodY = rand() % (MAX7219_SEG_NUM * 8); // Food y-coordinate

    Direction currentDirection = RIGHT;

    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize max7219 display
    adc_init(); // Initialize ADC for joystick

    pinMode(VERT_PIN, INPUT); // Set vertical joystick pin as input
    pinMode(HORZ_PIN, INPUT); // Set horizontal joystick pin as input
    pinMode(SEL_PIN, INPUT_PULLUP); // Set selection button pin as input with pull-up

    //max7219b_clrAll(); // Clear the display

    while(1) 
    {
        //max7219b_clrAll(); // Clear the display before updating position

        int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
        int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

        if (vert < 300 && currentDirection != DOWN) {
            y = y < (MAX7219_SEG_NUM * 8 - 1) ? y + 1 : (MAX7219_SEG_NUM * 8 - 1);
            
        }
        if (vert > 700 && currentDirection != UP) {
            y = y > 0 ? y - 1 : 0;
            currentDirection = DOWN;
        }
        if (horz > 700 && currentDirection != LEFT) {
            x = x < (MAX7219_SEG_NUM * 8 - 1) ? x + 1 : (MAX7219_SEG_NUM * 8 - 1);
            currentDirection = RIGHT;
        }
        if (horz < 300 && currentDirection != RIGHT) {
            x = x > 0 ? x - 1 : 0;
            currentDirection = LEFT;
        }
        if (x == foodX && y == foodY) {
            foodX = rand() % (MAX7219_SEG_NUM * 8);
            foodY = rand() % (MAX7219_SEG_NUM * 8);
        }


        max7219b_clrAll(); // Clear the display
        max7219b_set(y, x); // Set the pixel at current (x, y) location
        max7219b_set(foodY, foodX); // Display the food
        max7219b_out(); // Output the updated buffer to display

        _delay_ms(50); // Delay for 100 ms for smoother movement
    }

    return 0;
}