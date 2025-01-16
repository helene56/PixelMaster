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
    // rotate pieces
    constexpr int ROTATE {21};

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

        gpio_init(ROTATE);
        gpio_set_dir(ROTATE, GPIO_IN);
        gpio_pull_up(ROTATE);
    }

} // namespace control_buttons

void initialize_control_buttons() { control_buttons::initialize_control_buttons(); }

bool control_right()
{
    static bool last_state = true; // Assume button starts unpressed
    bool current_state     = gpio_get(control_buttons::RIGTH) == 0; // Active low

    if (current_state && !last_state)                               // Detect falling edge
    {
        last_state = current_state;
        return true;                                                // Button was just pressed
    }
    else
    {
        last_state = current_state;
        return false; // No new press detected
    }
}

bool control_left()
{
    // active low, is pulled up
    static bool last_state = true; // Assume button starts unpressed
    bool current_state     = {gpio_get(control_buttons::LEFT) == 0}; // Active low

    if (current_state && !last_state)                               // Detect falling edge
    {
        last_state = current_state;
        return true;                                                // Button was just pressed
    }
    else
    {
        last_state = current_state;
        return false; // No new press detected
    }
}

void move_piece(Pattern::Tetrispiece &piece, int *current_cols, bool &right_pressed,
                bool &left_pressed)
{
    // TODO: should rename piece.max_cols, right now it is only 2.. should be 4?
    if (right_pressed)
    {
        if (piece.right_border < 8)
        {
            for (int i = 0; i < piece.max_rows; ++i)
            {
                if (current_cols[i] != 0)
                {
                    current_cols[i] += 1;
                }
                
            }
            ++piece.right_border;
            ++piece.left_border;
            ++piece.mid_pixel;

            if (piece.current_pattern != Pattern::LAST_PATTERN)
            {
                // update the column patterns to reflect the move
                ++piece.pattern_counter;
            }
        }
        // return right_pressed to its original state, button no longer pressed
        right_pressed = false;
    }
    else if (left_pressed)
    {
        if (piece.left_border > 1)
        {
            for (int i = 0; i < piece.max_rows; ++i)
            {
                if (current_cols[i] != 0)
                {
                    current_cols[i] -= 1;
                }
            }
            --piece.left_border;
            --piece.right_border;
            --piece.mid_pixel;
            // update the column patterns to reflect the move
            if (piece.current_pattern != Pattern::LAST_PATTERN)
            {
                // update the column patterns to reflect the move
                --piece.pattern_counter;
            }
        }
        left_pressed = false;
    }
}

void rotate_piece(Pattern::Tetrispiece &piece, int *current_cols, bool &rotate_press)
{
    if (rotate_press)
    {
        switch (piece.id)
        {
        case 3:
            
            break;
        
        default:
            break;
        }
    }
}