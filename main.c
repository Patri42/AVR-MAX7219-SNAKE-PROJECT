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

#include "max72xx.h"
#include <stdio.h>
#include <util/delay.h>

// Define constants for joystick operation
#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

// Define macros for digital read/write operations (works only for PORTD)
#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD

#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin), (mode == INPUT_PULLUP ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin)))))

// Define constants for pin mode selection
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

// Define thresholds for joystick readings
#define JOY_THRESHOLD_LOW 300
#define JOY_THRESHOLD_HIGH 700

// Define prescaler division for ADC 
#define ADC_PRESCALER_DIVISION (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) // ADC prescaler division=128 (16Mhz/128=125Khz)

// Externally declared buffer for max7219
extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];


// Function to clear max7219 buffer
void max7219b_clrAll(void)
{
    for (uint8_t i = 0; i < MAX7219_BUFFER_SIZE; i++) {
        max7219_buffer[i] = 0;
    }
}


// Function to read analog value from specified pin
uint16_t readAnalog(uint8_t pin)
{
    ADMUX = (ADMUX & 0xF8) | (pin & 0x07); // select the ADC channel
    ADCSRA |= (1 << ADSC); // start the conversion
    while (ADCSRA & (1 << ADSC)); // wait for the conversion to finish
    return ADCW; // return the ADC value
}

// Function to initialize ADC 
void adc_init()
{
    ADMUX = (1 << REFS0); // reference voltage on AVCC
    ADCSRA = (1 << ADEN) | ADC_PRESCALER_DIVISION; // ADC enabled, prescaler division=128 (16Mhz/128=125Khz)
}



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

        if (vert < 300) {
            y = y < (MAX7219_SEG_NUM * 8 - 1) ? y + 1 : (MAX7219_SEG_NUM * 8 - 1); // Increase y if joystick moves up and within display limit
        }
        if (vert > 700) {
            y = y > 0 ? y - 1 : 0; // Decrease y if joystick moves down and within display limit
        }
        if (horz > 700) {
            x = x < (MAX7219_SEG_NUM * 8 - 1) ? x + 1 : (MAX7219_SEG_NUM * 8 - 1); // Increase x if joystick moves right and within display limit
        }
        if (horz < 300) {
            x = x > 0 ? x - 1 : 0; // Decrease x if joystick moves left and within display limit
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