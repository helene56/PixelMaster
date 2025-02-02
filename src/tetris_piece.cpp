#include "tetris_piece.h"
#include "led_control.h"
#include "led_memory.h"
#include "pico/stdlib.h"
#include "pio_utils.h"
#include <iostream>
#include <stdio.h>

#include "TetrisPatterns.h"
#include "control_piece.h"

namespace randomv
{
    // todo: seed with the current time/clock the application started up, to ensure randomness.
    std::uint16_t seed {42};
    int ran_num {random_generator(seed, 5)};
} // namespace randomv

namespace Time
{
    uint32_t interval               = 800;
    static uint32_t last_frame_time = 0;
    static uint32_t current_time    = to_ms_since_boot(get_absolute_time());
    //  frames::last_frame_time = current_time;
    bool switch_frame = (current_time - last_frame_time >= interval);
} // namespace Time

bool time_to_switch_frame()
{
    return (Time::current_time - Time::last_frame_time >= Time::interval);
}

int random_generator(std::uint16_t &X, int range)
{
    X ^= (X << 13); // XOR X with itself shifted left by 13 bits
    X ^= (X >> 17); // XOR result with itself shifted right by 17 bits
    X ^= (X << 5);  // XOR result with itself shifted left by 5 bits

    int ran_result = X % range;
    X ^= (X >> 5);  // Update seed to prepare for next call
    // return X % range; // range of num 0-4
    return ran_result;
}

int play_piece(Pattern::Tetrispiece &piece)
{
    Time::current_time = to_ms_since_boot(get_absolute_time());
    // boolean  to keep track of when the first normal pattern has been set
    static bool normalPatternSet {false};
    // static int counter {0};
    static bool right_has_been_pressed {
        false}; // is reset in move_piece. maybe this is not the best idea?
    static bool left_has_been_pressed {false};
    static bool rotate_has_been_pressed {false};
    volatile bool right_press {control_right()};
    volatile bool left_press {control_left()};
    volatile bool rotate_press {rotate()};
    // this solved the issue, now the button responds, but should probably refactor..
    if (right_press)
    {
        right_has_been_pressed = true;
    }
    else if (left_press)
    {
        left_has_been_pressed = true;
    }
    else if (rotate_press)
    {
        rotate_has_been_pressed = true;
    }

    int(*current_pattern_array)[4][2] = piece.collection_patterns[static_cast<int>(
        piece.current_pattern)]; // dereference to get the correct pattern
    if (time_to_switch_frame() || left_has_been_pressed || right_has_been_pressed)
    {
        // check to see if the piece is blocked, then stop moving
        if (piece_stop_moving(piece))
        {
            return -1;
        }
        printf("Inside time_to_switch_frame\n");
        printf("right_press: %d\n", right_press);
        int static current_rows[piece.max_rows] {0};
        int static current_cols[piece.max_rows] {0};

        if (piece.current_pattern == Pattern::PATTERN1)
        {

            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_rows[i] = (*current_pattern_array)[i][0];
                current_cols[i] = (*current_pattern_array)[i][1] + piece.pattern_counter;
            }
            rotate_piece(piece, current_rows, current_cols, rotate_has_been_pressed);
            move_piece(piece, current_cols, right_has_been_pressed, left_has_been_pressed);
            call_frame(current_rows, current_cols, piece.max_rows, piece.color);
            piece.current_pattern = switch_pattern(piece);

            Time::last_frame_time = Time::current_time;
        }
        else if (piece.current_pattern != Pattern::LAST_PATTERN)
        {
            clear_frame(current_rows, current_cols, piece.max_rows);
            // clear pixels on display
            clear_all_pixels();

            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_rows[i] = (*current_pattern_array)[i][0];
                current_cols[i] = (*current_pattern_array)[i][1] + piece.pattern_counter;
            }
            rotate_piece(piece, current_rows, current_cols, rotate_has_been_pressed);
            move_piece(piece, current_cols, right_has_been_pressed, left_has_been_pressed);
            call_frame(current_rows, current_cols, piece.max_rows, piece.color);
            // update time and pattern
            piece.current_pattern = switch_pattern(piece);
            Time::last_frame_time = Time::current_time;
        }
        // if the current row is 0, it should not do anything more
        // if last pattern
        else if (piece.current_row > 0 && piece.current_pattern == Pattern::LAST_PATTERN)
        {
            piece.pattern_counter = 0;
            // clear previous patterns
            // clear first frame here
            clear_frame(current_rows, current_cols, piece.max_rows);
            // clear pixels on display
            clear_all_pixels();
            // move pattern down
            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_rows[i] -= 1;
            }
            rotate_piece(piece, current_rows, current_cols, rotate_has_been_pressed);
            move_piece(piece, current_cols, right_has_been_pressed, left_has_been_pressed);
            call_frame(current_rows, current_cols, piece.max_rows, piece.color);

            // update time
            Time::last_frame_time = Time::current_time;
        }
        // go to next row
        --piece.current_row;
    }
    return piece.current_row;
}

void generate_piece()
{
    static Pattern::Tetrispiece piece11;
    static Pattern::Tetrispiece piece22;
    static Pattern::Tetrispiece piece33;
    if (!Pattern::initialized_piece)
    {
        Pattern::initializePiece1(piece11);
        Pattern::initializePiece2(piece22);
        Pattern::initializePiece3(piece33);
    }

    static int result {1};

    if (result <= 0 && !game_end())
    {
        randomv::ran_num = random_generator(randomv::seed, 5);
        result           = 1;
        // reset the pieces to the start values
        Pattern::reset_piece1(piece11);
        Pattern::reset_piece2(piece22);
        Pattern::reset_piece3(piece33);
    }

    switch (randomv::ran_num)
    {
    case 0:
        result = play_piece(piece22);
        break;
    case 1:
        result = play_piece(piece22);
        break;
    case 2:
        result = play_piece(piece22);
        break;
    case 3:
        result = play_piece(piece22);
        break;
    case 4:
        result = play_piece(piece22);
        break;

    default:
        break;
    }
    if (!result)
    {
        win_row();
    } 
    
}

// refactor these two functions under here somehow, they almost do the same
void call_frame(int *rows, int *cols, int size, std::int32_t grb)
{
    int *p             = rows;
    int *p2            = cols;
    std::uint8_t green = (grb >> 16) & 0xFF; // Extract the red component (8 bits)
    std::uint8_t red   = (grb >> 8) & 0xFF;  // Extract the green component (8 bits)
    std::uint8_t blue  = grb & 0xFF;         // Extract the blue component (8 bits)

    for (int i = 0; i < size; ++i)
    {
        // check to assure no 0 is passed
        if (*p != 0 || *p2 != 0)
        {
            storeLed(*p, *p2, green, red, blue);
            ++p;
            ++p2;
        }
    }
    sendLed();
}

void store_latestLed(int *rows, int *cols, int size, std::int32_t grb)
{
    std::uint8_t green = (grb >> 16) & 0xFF; // Extract the red component (8 bits)
    std::uint8_t red   = (grb >> 8) & 0xFF;  // Extract the green component (8 bits)
    std::uint8_t blue  = grb & 0xFF;         // Extract the blue component (8 bits)
    int *p             = rows;
    int *p2            = cols;
    for (int i = 0; i < size; ++i)
    {
        storeLed(*p, *p2, green, red, blue);
        ++p;
        ++p2;
    }
}

// clear the rows and cols in ledmemory
void clear_frame(int *rows, int *cols, size_t size)
{
    int *p  = rows;
    int *p2 = cols;
    for (int i = 0; i < size; ++i)
    {
        if (*p != 0 || *p2 != 0)
        {
            clear_Ledmemory(*p, *p2);
        }
        ++p;
        ++p2;
    }
}

bool check_Ledplacement(int row, int col) { return (led_memory[row - 1][col - 1] > 0); }

bool game_end()
{
    // consider refactoring this, so the boolean is not necessary.
    // simply return true or false.
    bool game_off {true};
    for (int col = 0; col < 8; ++col)
    {
        for (int row = 0; row < 8; ++row)
        {
            if (led_memory[row][col] == 0)
            {
                game_off = false;
                break;
            }
        }
    }
    return game_off;
}

struct complete_line
{
    int complete_row[8] {0};
};

struct complete_line complete_row()
{
    struct complete_line resulting_line;

    for (int row = 0; row < 8; ++row)
    {
        bool row_complete = true; // Reset row_complete for each row
        for (int col = 0; col < 8; ++col)
        {
            if (led_memory[row][col] == 0)
            {
                // if it find one column not lit up, the row is not completed.
                row_complete = false;
                break;
            }
        }
        if (row_complete)
        {
            resulting_line.complete_row[row] = 1;
        }
    }

    return resulting_line;
}

// remove row, player wins a row, add score point
void win_row()
{
    bool player_win {false};
    struct complete_line result_row
    {
        complete_row()
    };
    int test_cols[8] {1, 2, 3, 4, 5, 6, 7, 8};
    // fill this in with the row that was complete
    // start with only one row completion
    int selected_rows[8] {0};
    for (int i {0}; i < 8; ++i)
    {

        if (result_row.complete_row[i] != 0)
        {
            int select_i = ++i;
            for (int j {0}; j < 8; ++j)
            {
                selected_rows[j] = select_i;
            }
            player_win = true;
            // break for now; later implement check for multiple rows
            break;
        }
    }

    // for (int i {0}, j{1}; i < 8; ++i, ++j)
    // {

    //     if (result_row.complete_row[i] != 0)
    //     {
    //         int select_i = ++i;
    //         for (int j {0}; j < 8; ++j)
    //         {
    //             selected_rows[j] = select_i;
    //         }
    //         player_win = true;
    //         // break for now; later implement check for multiple rows
    //         break;
    //     }
    //     else
    //     {
    //         break;
    //     }
    // }

    if (player_win)
    {
        // pass the select_i instead and fill array with that to send instead
        // maybe only check this when a piece stops moving?
        clear_frame(selected_rows, test_cols, 8);
        // now: move all pixels above one row down
        // assign just the first for now, they should all be the same
        int select_row {selected_rows[0]};
        // for now try to only change led_memory, next time it is called, it will be moved down when
        // time to switch frame

        // only loop through rows above the completed row
        for (int i {select_row - 1}; i < 8; ++i)
        {
            for (int j {0}; j < 8; ++j)
            {
                // if led memory is not 0 and the value is the row above the deleted row
                if (led_memory[i][j] != 0)
                {
                    // first assign pixel to the row under it and same col
                    led_memory[i - 1][j] = led_memory[i][j];
                    // reset the previous
                    led_memory[i][j] = 0;
                }
            }
        }
    }
}