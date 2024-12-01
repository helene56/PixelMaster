#ifndef TETRIS_PIECE 
#define TETRIS_PIECE

#include <cstdint>

int random_generator(std::uint16_t X, int range);
void generate_piece();
int piece1();
void piece2();
bool check_Ledplacement(int row, int col);

#endif // TETRIS_PIECE