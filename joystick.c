#include "joystick.h"
#include "millis.h"

void joystick_init(void) {
    pinMode(VERT_PIN, INPUT);
    pinMode(HORZ_PIN, INPUT);
    pinMode(SEL_PIN, INPUT_PULLUP);
    adc_init();
}

// Function to read analog value from specified pin
uint16_t analogRead(uint8_t pin)
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

void wait_for_select_button() {
//     // Initial time stamp when the function is entered
//     millis_t startTime = millis_get();

//     // Debounce time in milliseconds
//     const millis_t debounceTime = 10;

//     // Wait until the button is pressed
//     while(digitalRead(SEL_PIN) == HIGH) {
//         // Check if debounce time has passed
//         if (millis_get() - startTime >= debounceTime) {
//             startTime = millis_get();  // Update the start time
//         }
//     }

//     // Update the start time for button release
//     startTime = millis_get();

//     // Optional: wait for button release to avoid repeated action on single press
//     while(digitalRead(SEL_PIN) == LOW) {
//         // Check if debounce time has passed
//         if (millis_get() - startTime >= debounceTime) {
//             startTime = millis_get();  // Update the start time
//         }
//     }
// }
    // Debounce time in milliseconds
    const millis_t debounceTime = 10;

    // Wait until the button is pressed
    while(digitalRead(SEL_PIN) == HIGH) {
    }

    // Button is pressed, wait for debounce time to confirm
    millis_t pressedTime = millis_get();
    while (millis_get() - pressedTime < debounceTime) {
    }

    // Wait for button release
    while(digitalRead(SEL_PIN) == LOW) {
    }

    // Button is released, wait for debounce time to confirm
    millis_t releasedTime = millis_get();
    while (millis_get() - releasedTime < debounceTime) {
    }
}




