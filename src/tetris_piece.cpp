#include <stdio.h>
#include "led_control.h"

// steps
// generate random piece at random location, start in the top of the matrix
// move it down till it reaches the bottom

int random_generator(std::uint16_t X, int range)
{
    X ^= (X << 13);  // XOR X with itself shifted left by 13 bits
    X ^= (X >> 17);  // XOR result with itself shifted right by 17 bits
    X ^= (X << 5);   // XOR result with itself shifted left by 5 bits

    return X % range; // range of num 0-4
}

void generate_piece(std::uint8_t green, std::uint8_t red, std::uint8_t blue)
{
    std::uint16_t seed = 42;
    std::uint16_t col_seed = 53;

    int ran_num = random_generator(seed, 5);

    switch (ran_num)
    {
        case 0:
        case 1:

        case 2:
            int col = random_generator(col_seed, 8);
            ++col;
            storeLed(8, col, 0b00001101, 0b00001101, 0b00000000);
            storeLed(7, col, 0b00001101, 0b00001101, 0b00000000);
            break;
        case 3:
            int col = random_generator(col_seed, 8);
            ++col;
            storeLed(8, col, 0b00001101, 0b00001101, 0b00000000);
            storeLed(7, col, 0b00001101, 0b00001101, 0b00000000);

        case 4:
        

        
        default:
            break;
    }
}