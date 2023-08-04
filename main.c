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

#define VERT_THRESHOLD_LOW 300
#define VERT_THRESHOLD_HIGH 700
#define HORZ_THRESHOLD_LOW 300
#define HORZ_THRESHOLD_HIGH 700

int x = 0;
int y = 0;

int readAnalog(uint8_t pin) {
    // TODO: write code here to read from the ADC
    return 0;  // placeholder return value
}

void setPoint(int x, int y) {
    // Clear the display buffer
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            max7219b_clr(i, j);
    }
}

    // Set the point in the buffer
    max7219b_set(x, y);
    // Output the buffer to the display
    max7219b_out();
}

int main ()
    {
        init_serial();
        max7219_init();

        pinMode(VERT_PIN, INPUT);
	    pinMode(HORZ_PIN, INPUT);
	    pinMode(SEL_PIN, INPUT_PULLUP);


        while (1) {  // main loop
            int vert = readAnalog(VERT_PIN);
            int horz = readAnalog(HORZ_PIN);
            if (vert < VERT_THRESHOLD_LOW) {
                y = y > 0 ? y - 1 : 0;
            }
            if (vert > VERT_THRESHOLD_HIGH) {
                y = y < 7 ? y + 1 : 7;
            }
            if (horz < HORZ_THRESHOLD_LOW) {
                x = x > 0 ? x - 1 : 0;
            }
            if (horz > HORZ_THRESHOLD_HIGH) {
                x = x < 7 ? x + 1 : 7;
            }
            if (!digitalRead(SEL_PIN)) {
                x = y = 0;
            }
            setPoint(x, y);
            _delay_ms(100);
        }
    return 0;
    }
