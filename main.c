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

int x = 0;
int y = 0;

int main ()
    {
        init_serial();
        max7219_init();

        pinMode(VERT_PIN, INPUT);
	    pinMode(HORZ_PIN, INPUT);
	    pinMode(SEL_PIN, INPUT_PULLUP);

    }