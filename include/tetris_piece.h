#ifndef TETRIS_PIECE 
#define TETRIS_PIECE

#include <cstdint>
#include <stdio.h>
int random_generator(std::uint16_t &X, int range);
void generate_piece();
bool check_Ledplacement(int row, int col);
void clear_frame(int *rows, int *cols, size_t size);
void call_frame(int *rows, int *cols, int size, std::int32_t grb);
bool game_end();
struct complete_line complete_row();

#endif // TETRIS_PIECE