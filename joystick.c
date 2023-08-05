#include "joystick.h"

void joystick_init(void) {
    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);
    adc_init();
}

uint16_t readAnalog(uint8_t pin) {
    ADMUX = (ADMUX & 0xF8) | (pin & 0x07);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADCW;
}

void adc_init() {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | ADC_PRESCALER_DIVISION;
}