#include "max72xx.h"

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pin_defs.h" 

// Define constants for joystick operation
#define VERT_PIN PC0 //  A0
#define HORZ_PIN PC1 //  A1
#define SEL_PIN PD2  //  D2

// Define constants for pin mode selection
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2

#define LOW 0
#define HIGH 1

// Define thresholds for joystick readings
#define JOYSTICK_THRESHOLD_LOW 300
#define JOYSTICK_THRESHOLD_HIGH 700

// Define prescaler division for ADC 
#define ADC_PRESCALER_DIVISION (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) // ADC prescaler division=128 (16Mhz/128=125Khz)

void joystick_init(void);
uint16_t readAnalog(uint8_t pin);
void wait_for_select_button(void);

#endif 