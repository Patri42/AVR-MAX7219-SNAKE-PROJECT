#include "max72xx.h"
#include <stdio.h>
#include <util/delay.h>
// #include <avr/io.h> **already exists in header

#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD
//#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin))) // Only for PORTD

#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin), (mode == INPUT_PULLUP ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin)))))


#define INPUT 0
#define OUTPUT 1
# define INPUT_PULLUP 2

//#define MAX_DEVICES	1

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

int x = 0;
int y = 0;

int main ()
{
    init_serial();
    max7219_init();
    adc_init();

    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);
}
