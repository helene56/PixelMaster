#ifndef CONTROL_PIECE 
#define CONTROL_PIECE

#include "TetrisPatterns.h"

void initialize_control_buttons();
void move_piece(Pattern::Tetrispiece &piece, int *current_rows, bool right_pressed, bool left_pressed);
bool control_right();
bool control_left();

#endif // CONTROL_PIECE