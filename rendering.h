#ifndef RENDERING_H
#define RENDERING_H

#include "game_logic.h" // Include the game logic header to have access to Game structure
#include "max72xx.h"

extern uint8_t letter_G[MAX7219_BUFFER_SIZE];
extern uint8_t letter_A[MAX7219_BUFFER_SIZE];
extern uint8_t letter_M[MAX7219_BUFFER_SIZE];
extern uint8_t letter_E[MAX7219_BUFFER_SIZE];

// Function to render the game, including snake and food
void render_game(Game* game);

void render_message();

void shift_message_left();

// Declare the function to render the game over message
void render_game_over_message();

#endif // RENDERING_H