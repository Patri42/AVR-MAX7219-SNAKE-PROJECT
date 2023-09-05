#include "max72xx.h"
#include "pin_defs.h"

#ifndef DISPLAY_H
#define DISPLAY_H

// Declares that max7219_buffer is an array of a certain size that is defined elsewhere
extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

void max7219b_clrAll(void);
void display_init(void);
void display_set(uint8_t x, uint8_t y);
void display_output(void);

#endif 