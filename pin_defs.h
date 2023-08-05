#ifndef PIN_DEFS_H
#define PIN_DEFS_H

#include <avr/io.h>

// Define macros for digital read/write operations (works only for PORTD)
#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD

#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin), (mode == INPUT_PULLUP ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin)))))

// Define prescaler division for ADC 
#define ADC_PRESCALER_DIVISION (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) // ADC prescaler division=128 (16Mhz/128=125Khz)


#endif