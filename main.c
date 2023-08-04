#include "max72xx.h"
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

#define digitalRead(pin) (!!(PIND & (1 << pin))) // Only for PORTD
#define digitalWrite(pin, value) (value ? (PORTD |= (1 << pin)) : (PORTD &= ~(1 << pin))) // Only for PORTD
#define pinMode(pin, mode) (mode == OUTPUT ? (DDRD |= (1 << pin)) : (DDRD &= ~(1 << pin))) // Only for PORTD