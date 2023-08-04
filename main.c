#include "max72xx.h"
#include <stdio.h>
#include <util/delay.h>
// #include <avr/io.h> **already exists in header

extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD
//#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin))) // Only for PORTD

#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin), (mode == INPUT_PULLUP ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin)))))


#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

#define JOY_DEADZONE 20
#define JOY_MAX 1023

//#define MAX_DEVICES	1

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint16_t readAnalog(uint8_t pin)
{
    ADMUX = (ADMUX & 0xF8) | (pin & 0x07); // select the ADC channel
    ADCSRA |= (1 << ADSC); // start the conversion
    while (ADCSRA & (1 << ADSC)); // wait for the conversion to finish
    return ADCW; // return the ADC value
}

void adc_init()
{
    ADMUX = (1 << REFS0); // reference voltage on AVCC
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // ADC enabled, prescaler division=128 (16Mhz/128=125Khz)
}



int main ()
{
    init_serial();
    max7219_init();
    adc_init();



    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);

    int last_x = 0;
    int last_y = 0;

    int x = 1023 - readAnalog(HORZ_PIN);
    int y = 1023 - readAnalog(VERT_PIN);

   while(1) 
    {
        int raw_x = readAnalog(HORZ_PIN);
        int raw_y = readAnalog(VERT_PIN);

        // Convert the readings to LED matrix positions
        int x = raw_x / 128;
        int y = raw_y / 128;

        // Check if the position has changed significantly
        if(abs(x - last_x) > 64 || abs(y - last_y) > 64) 
        {
            // Update the last known positions
            last_x = x;
            last_y = y;

            // Clear the previous LED
            max7219b_clr(7 - last_y, last_x);  // Clear the previously set pixel

            // Update the LED position
            y = 7 - map(raw_y, 0, 1023, 0, 7); // Reverse direction so up is up
            x = map(raw_x, 0, 1023, 0, 7);     // No need to reverse the x-axis
            max7219b_set(y, x);                // Set the new pixel

            _delay_ms(10); // avoid rapid refresh
        }

        if (!digitalRead(SEL_PIN)) 
        {
            max7219b_clr(y, x);   // Clear the LED if the joystick is pressed
        }

        max7219b_out();
    }

    return 0;
}