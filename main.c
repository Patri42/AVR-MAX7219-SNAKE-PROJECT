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

// Wokwi:
// https://wokwi.com/projects/296234816685212169

#include <avr/interrupt.h>
#include <avr/io.h>
#include "joystick.h"
#include "display.h"
//#include <stdlib.h> 
//#include <util/delay.h>
//#include <stdbool.h>
#include <time.h>
#include "millis.h"
#include "uart.h"
//#include "debug.h"
#include "game_logic.h"

int main(void) {
    
    millis_init(); // Initialize millis
    sei(); // Enable global interrupts

    srand(time(NULL));
    //srand(analogRead(0));
    init_serial(); // Initialize serial communication
    max7219_init(); // Initialize the display (if applicable)
    adc_init(); // Initialize ADC for joystick

    //render_game_over_message();

    //wait_for_select_button();

    game_loop();     // Start the game loop
    //render_game_over_message(); <-- Already exists in gameloop when game over
    return 0;        // Return code
}