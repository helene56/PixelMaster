// module to control the tetris pieces created in tetris_piece.cpp
#include "pico/stdlib.h"

#include "TetrisPatterns.h"
#include "tetris_piece.h"

namespace control_buttons
{
    // for speed up
    constexpr int DOWN {20};
    constexpr int RIGTH {19};
    constexpr int LEFT {18};

    // initialize buttons
    void initialize_control_buttons()
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
    }

} // namespace control_buttons

void initialize_control_buttons() { control_buttons::initialize_control_buttons(); }

bool control_right()
{
    // active low, is pulled up
    return {gpio_get(control_buttons::RIGTH) == 0};
}

bool control_left()
{
    // active low, is pulled up
    return {gpio_get(control_buttons::LEFT) == 0};
}

void move_piece(Pattern::Tetrispiece &piece, int *current_cols, bool right_pressed,
                bool left_pressed)
{
    // TODO: should rename piece.max_cols, right now it is only 2.. should be 4?
    // current_col is set to be the middle piece which is why it wont work for pieces filling more
    // than one pixel
    if (right_pressed)
    {
        printf("right is pressed\n");
        if (piece.current_col < 8)
        {
            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_cols[i] += 1;
            }
            ++piece.current_col;
        }
    }
    else if (left_pressed)
    {
        printf("left is pressed\n");
        if (piece.current_col > 0)
        {
            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_cols[i] -= 1;
            }
            --piece.current_col;
        }
    }
}