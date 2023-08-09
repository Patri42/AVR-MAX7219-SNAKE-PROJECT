#ifndef RENDERING_H
#define RENDERING_H

#include "game_logic.h" // Include the game logic header to have access to Game structure

// Function to render the game, including snake and food
void render_game(Game* game);

// Declare the function to render the game over message
void render_game_over_message();

#endif // RENDERING_H