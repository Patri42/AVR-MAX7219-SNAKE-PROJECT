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
#include <util/delay.h>

int main () {
    int x = 0; // Initialize x position
    int y = 0; // Initialize y position

    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize max7219 display
    adc_init(); // Initialize ADC for joystick

    pinMode(VERT_PIN, INPUT); // Set vertical joystick pin as input
    pinMode(HORZ_PIN, INPUT); // Set horizontal joystick pin as input
    pinMode(SEL_PIN, INPUT_PULLUP); // Set selection button pin as input with pull-up

    while(1) 
    {
        int horz = 1023 - readAnalog(HORZ_PIN); // Read horizontal joystick value
        int vert = 1023 - readAnalog(VERT_PIN); // Read vertical joystick value

        if (horz > 700) {
            y = y < (MAX7219_SEG_NUM * 8 - 1) ? y + 1 : (MAX7219_SEG_NUM * 8 - 1); // Increase x if joystick moves right and within display limit
        }
        if (horz < 300) {
            y = y > 0 ? y - 1 : 0; // Decrease x if joystick moves left and within display limit
        }
        if (vert > 700) {
            x = x < (MAX7219_SEG_NUM * 8 - 1) ? x + 1 : (MAX7219_SEG_NUM * 8 - 1); // Increase y if joystick moves up and within display limit
        }
        if (vert < 300) {
            x = x > 0 ? x - 1 : 0; // Decrease y if joystick moves down and within display limit
        }
      
        if (!digitalRead(SEL_PIN)) 
        {
            max7219b_clrAll(); // Clear the display if the selection button is pressed
            max7219b_out(); // Output the cleared buffer to display
        }
        max7219b_set(y, x); // Set the pixel at current (x, y) location
        max7219b_out(); // Output the updated buffer to display

        _delay_ms(100); // Delay for 100 ms for debouncing
    }

    return 0;
}