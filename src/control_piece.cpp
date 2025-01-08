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

void initialize_control_buttons()
{
    control_buttons::initialize_control_buttons();
}

void move_piece(Pattern::Tetrispiece &piece, int current_rows[4])
{
    // active low, is pulled up
    volatile bool right_pressed {gpio_get(control_buttons::RIGTH) == 0};
    volatile bool left_pressed {gpio_get(control_buttons::LEFT) == 0};
    volatile bool down_pressed {gpio_get(control_buttons::DOWN) == 0};

    int(*current_pattern_array)[4][2] = piece.collection_patterns[static_cast<int>(
        piece.current_pattern)]; // dereference to get the correct pattern
    
    

    if (right_pressed)
    {
        if (piece.current_col < 8)
        {
            for (int i = 0; i < piece.max_cols; ++i)
            {
                current_rows[i] += 1;
            }
            ++piece.current_col;
        }
        
    }
    else if (left_pressed)
    {
        if (piece.current_col > 0)
        {
            for (int i = 0; i < piece.max_cols; ++i)
            {
                current_rows[i] -= 1;
            }
            --piece.current_col;
        }   
    }
    
}

void apply_move(Pattern::Tetrispiece &piece, int current_rows[4])
{
    int(*current_pattern_array)[4][2] = piece.collection_patterns[static_cast<int>(
        piece.current_pattern)]; // dereference to get the correct pattern
    
    for (int i = 0; i < piece.max_cols; ++i)
        {
            current_rows[i] -= 1;
        }
}