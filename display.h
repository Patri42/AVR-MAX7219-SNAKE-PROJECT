#include "max72xx.h"
#include "pin_defs.h"

#ifndef DISPLAY_H
#define DISPLAY_H

extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

void max7219b_clrAll(void);
void display_init(void);
void display_set(int x, int y);
void display_output(void);

#endif // DISPLAY_H