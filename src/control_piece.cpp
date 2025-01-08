// module to control the tetris pieces created in tetris_piece.cpp
#include "pico/stdlib.h"

#include "tetris_piece.h"
#include "TetrisPatterns.h"

namespace control_buttons
{
    // for speed up
    constexpr int DOWN {20};
    constexpr int RIGTH {19};
    constexpr int LEFT {18};

    // initialize buttons
    bool initialize_control_buttons()
    {
        gpio_init(DOWN);
        gpio_set_dir(DOWN, GPIO_IN);
        gpio_pull_up(DOWN);

        gpio_init(RIGTH);
        gpio_set_dir(RIGTH, GPIO_IN);
        gpio_pull_up(RIGTH);

        gpio_init(LEFT);
        gpio_set_dir(LEFT, GPIO_IN);
        gpio_pull_up(LEFT);

        return true;
    }
 
} // namespace control_buttons



void move_piece(Pattern::Tetrispiece &piece)
{
    if (!control_buttons::initialize_control_buttons());
}