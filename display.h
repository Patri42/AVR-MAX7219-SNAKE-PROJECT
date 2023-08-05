#include "max72xx.h"

#ifndef DISPLAY_H
#define DISPLAY_H

#include "pin_defs.h"

extern uint8_t max7219_buffer[MAX7219_BUFFER_SIZE];

void display_init(void);
void display_clear(void);
void display_set(int x, int y);
void display_output(void);

#endif // DISPLAY_H