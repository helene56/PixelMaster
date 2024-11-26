#ifndef TETRIS_PIECE 
#define TETRIS_PIECE

#include <cstdint>

int random_generator(std::uint16_t X, int range);
void generate_piece(std::uint8_t green, std::uint8_t red, std::uint8_t blue);
void piece1();

#endif // TETRIS_PIECE