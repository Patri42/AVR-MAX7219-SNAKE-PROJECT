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

#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD

#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin), (mode == INPUT_PULLUP ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin)))))


#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

#define JOY_DEADZONE 20
#define JOY_MAX 1023

#define JOY_THRESHOLD_LOW 300
#define JOY_THRESHOLD_HIGH 700

#define ADC_PRESCALER_DIVISION (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) // ADC prescaler division=128 (16Mhz/128=125Khz)

extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

void max7219b_clrAll(void)
{
    for (uint8_t i = 0; i < MAX7219_BUFFER_SIZE; i++) {
        max7219_buffer[i] = 0;
    }
}

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
    ADCSRA = (1 << ADEN) | ADC_PRESCALER_DIVISION; // ADC enabled, prescaler division=128 (16Mhz/128=125Khz)
}



int main () {
    int x = 0;
    int y = 0;

    init_serial();
    max7219_init();
    adc_init();

    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);

    while(1) 
    {
        int horz = 1023 - readAnalog(HORZ_PIN);
        int vert = 1023 - readAnalog(VERT_PIN);
        if (vert < JOY_THRESHOLD_LOW) {
            y = y < 7 ? y + 1 : 7;
        }
        if (vert > JOY_THRESHOLD_HIGH) {
            y = y > 0 ? y - 1 : 0;
        }
        if (horz > JOY_THRESHOLD_HIGH) {
            x = x < 15 ? x + 1 : 15;
        }
        if (horz < JOY_THRESHOLD_LOW) {
            x = x > 0 ? x - 1 : 0;
        }
        if (!digitalRead(SEL_PIN)) 
        {
            max7219b_clrAll();
            max7219b_out();
        }
        max7219b_set(y, x);
        max7219b_out();

        _delay_ms(100);
    }

    return 0;
}