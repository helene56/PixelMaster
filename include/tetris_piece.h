#ifndef TETRIS_PIECE 
#define TETRIS_PIECE

#include <cstdint>

int random_generator(std::uint16_t &X, int range);
void generate_piece();
int piece1();
int piece2();
bool check_Ledplacement(int row, int col);
void clear_frame(int *rows, int *cols, int size);
void call_frame(int *rows, int *cols, int size, std::int32_t grb);
bool game_end();

#endif // TETRIS_PIECE