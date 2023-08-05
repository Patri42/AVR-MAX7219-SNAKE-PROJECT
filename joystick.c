#include "joystick.h"

void joystick_init(void) {
    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);
    adc_init();
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

