#include "tetris_piece.h"
#include "led_control.h"
#include "led_memory.h"
#include "pico/stdlib.h"
#include "pio_utils.h"
#include <iostream>
#include <stdio.h>
// steps
// generate randomv piece at randomv location, start in the top of the matrix
// move it down till it reaches the bottom

namespace color
{
    std::uint32_t green = (0b00001101 << 16) | (0b00000000 << 8) | 0b00000000;

} // namespace color

namespace randomv
{
    std::uint16_t seed {42};
    int ran_num {random_generator(seed, 5)};
} // namespace randomv

namespace Pattern
{
    enum patterns_state
    {
        PATTERN1,
        PATTERN2,
        PATTERN3,
        PATTERN4,
        NORMAL,
        LAST_PATTERN = NORMAL // explicitly setting the last item
    };
    bool initialized_piece {false};
    // define the frame struct
    struct Tetrispiece
    {
        static constexpr int max_rows {4};
        static constexpr int max_cols {2};
        // initialize these variables
        int id {0};
        int current_row {0};
        int pattern1[max_rows][max_cols] {};
        int pattern2[max_rows][max_cols] {};
        int pattern3[max_rows][max_cols] {};
        int pattern4[max_rows][max_cols] {};
        int normal_pattern[max_rows][max_cols] {};
        std::uint32_t color {0};
        // do not initialize
        patterns_state current_pattern {PATTERN1};
        int (*collection_patterns[5])[4][2] {};
    };

    // a general function to stop pieces from moving.
    // provide an id to identify the specific conditon.
    bool piece_stop_moving(const Tetrispiece &piece)
    {
        switch (piece.id)
        {
        case 1:
            if (piece.current_pattern == PATTERN1)
            {
                return (check_Ledplacement(8, 4));
            }
            else if (piece.current_pattern == PATTERN2)
            {
                return (check_Ledplacement(8 - 1, 4));
            }
            else if (piece.current_row >= 0)
            {
                return ((check_Ledplacement(piece.current_row, 4) ||
                         check_Ledplacement(piece.current_row, 5) ||
                         check_Ledplacement(piece.current_row, 3)));
            }

        default:
            return false;
        }
    }

    // declare frames
    // piece1
    static int p1_row {6}; // consider passing this as a variable to functions
                           // instead of global variable
    int p1_id {1};
    void initializePiece(Tetrispiece &piece)
    {
        piece = {
            p1_id,  // id
            p1_row, // current row
            {
                // pattern1
                {8, 3}, // Row 1
                {8, 4}, // Row 2
                {8, 5}, // Row 3
                {0, 0}  // this will be initialized as 0.. need to rework the
                        // code to only work with + numbers
            },
            {           // pattern2
             {8, 4},
             {7, 3},
             {7, 4},
             {7, 5}},
            {0}, // pattern3
            {0}, // pattern4
            {    // normal
             {p1_row + 1, 4},
             {p1_row, 3},
             {p1_row, 4},
             {p1_row, 5}},
            color::green,
        };
        // piece is initialized
        initialized_piece            = true;
        piece.collection_patterns[0] = &piece.pattern1;
        piece.collection_patterns[1] = &piece.pattern2;
        piece.collection_patterns[2] = &piece.pattern3;
        piece.collection_patterns[3] = &piece.pattern4;
        piece.collection_patterns[4] = &piece.normal_pattern;
    }

    constexpr int patterns_state_count {6}; // including last pattern twice
    // returns the next pattern in order
    patterns_state switch_pattern(Tetrispiece &piece)
    {
        if (piece.current_pattern != LAST_PATTERN)
        {
            bool all_zero {true};
            int index                   = static_cast<int>(piece.current_pattern) + 1;
            int(*current_pattern)[4][2] = piece.collection_patterns[index];

            for (int i = 0; i < piece.max_rows; ++i)
            {
                if ((*current_pattern)[i][0] > 0 || (*current_pattern)[i][1] > 0)
                {
                    all_zero = false;
                }
            }
            // if it is not all 0, meaning there is a next pattern, return that pattern
            if (!all_zero)
            {
                return static_cast<patterns_state>((static_cast<int>(piece.current_pattern) + 1) %
                                                   patterns_state_count);
            }
            // if it is all 0, it means that there are no more unique patterns and it should be set
            // to normal
            else
            {
                return NORMAL;
            }
        }
        else
        {
            return NORMAL;
        }
    }

} // namespace Pattern

namespace Time
{
    uint32_t interval               = 800;
    static uint32_t last_frame_time = 0;
    static uint32_t current_time    = to_ms_since_boot(get_absolute_time());
    //  frames::last_frame_time = current_time;
    bool switch_frame = (current_time - last_frame_time >= interval);
} // namespace Time

namespace piece_settings
{
    enum Framestate
    {
        first_frame,
        second_frame,
        normal_frame
    };
    static Framestate current_frame {first_frame};
    static int current_row {6};
} // namespace piece_settings

bool time_to_switch_frame()
{
    return (Time::current_time - Time::last_frame_time >= Time::interval);
}

int random_generator(std::uint16_t &X, int range)
{
    X ^= (X << 13); // XOR X with itself shifted left by 13 bits
    X ^= (X >> 17); // XOR result with itself shifted right by 17 bits
    X ^= (X << 5);  // XOR result with itself shifted left by 5 bits

    int result = X % range;
    X ^= (X >> 5);  // Update seed to prepare for next call
    // return X % range; // range of num 0-4
    return result;
}

int play_piece(Pattern::Tetrispiece &piece)
{
    Time::current_time = to_ms_since_boot(get_absolute_time());
    // boolean  to keep track of when the first normal pattern has been set
    static bool normalPatternSet {false};

    int(*current_pattern_array)[4][2] = piece.collection_patterns[static_cast<int>(
        piece.current_pattern)]; // dereference to get the correct pattern
    if (time_to_switch_frame())
    {
        // check to see if the piece is blocked, then stop moving
        if (piece_stop_moving(piece))
        {
            return -1;
        }

        int static current_rows[piece.max_rows] {0};
        int static current_cols[piece.max_rows] {0};

        if (piece.current_pattern == Pattern::PATTERN1)
        {

            for (int i = 0; i < piece.max_rows; ++i)
            {
                current_rows[i] = (*current_pattern_array)[i][0];
                current_cols[i] = (*current_pattern_array)[i][1];
            }
            call_frame(current_rows, current_cols, piece.max_rows, piece.color);
            piece.current_pattern = switch_pattern(piece);
            Time::last_frame_time = Time::current_time;
        }
        // if the current row is 0, it should not do anything more
        else if (piece.current_row > 0)
        {
            // clear previous patterns
            // clear first frame here
            clear_frame(current_rows, current_cols, piece.max_rows);
            // clear pixels on display
            clear_all_pixels();
            // set new leds
            if (!normalPatternSet)
            {
                for (int i = 0; i < piece.max_rows; ++i)
                {
                    current_rows[i] = (*current_pattern_array)[i][0];
                    current_cols[i] = (*current_pattern_array)[i][1];
                }
                // normal pattern set once
                normalPatternSet = true;
            }
            else
            {
                // now subtract from the set rows to move the normal pattern down
                for (int i = 0; i < piece.max_rows; ++i)
                {
                    current_rows[i] -= 1;
                }
            }

            call_frame(current_rows, current_cols, piece.max_rows, piece.color);

            // depend on the pattern setting
            if (piece.current_pattern != Pattern::LAST_PATTERN)
            {
                // update pattern setting if it is a unique pattern
                piece.current_pattern = switch_pattern(piece);
            }
            // if normal/last
            else
            {
                // go to next row
                --piece.current_row;
            }
            // update time
            Time::last_frame_time = Time::current_time;
        }
        printf("current row: %d\n", piece.current_row);
    }
    return piece.current_row;
}

void generate_piece()
{
    static Pattern::Tetrispiece piece11;
    if (!Pattern::initialized_piece)
    {
        Pattern::initializePiece(piece11);
    }

    int(*current_pattern_array)[4][2] = piece11.collection_patterns[0];

    static int result {1};

    if (result <= 0 && !game_end())
    {
        // the piece that just ran should be reset now, but the last leds should
        // remain need to add some resets for the second piece as well.. maybe
        // some other way to keep it organized..
        piece_settings::current_frame = piece_settings::first_frame;
        piece_settings::current_row   = 6;
        randomv::ran_num              = random_generator(randomv::seed, 5);
        result                        = 1;
    }

    switch (randomv::ran_num)
    {
    case 0:
        // result = piece1();
        result = play_piece(piece11);
        break;
    case 1:
        // result = piece1();
        result = play_piece(piece11);
        break;
    case 2:
        // result = piece1();
        result = play_piece(piece11);
        break;
    case 3:
        // result = piece1();
        result = play_piece(piece11);
        break;
    case 4:
        // result = piece1();
        result = play_piece(piece11);
        break;

    default:
        break;
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
        clear_Ledmemory(*p, *p2);
        ++p;
        ++p2;
    }
}

int piece1()
{
    Time::current_time = to_ms_since_boot(get_absolute_time());

    // enum Framestate {first_frame, second_frame, normal_frame};
    // static Framestate current_frame {first_frame};

    // static int current_row {6};

    int first_frame_rows[3] {8, 8, 8};
    int first_frame_cols[3] {3, 4, 5};
    int second_frame_rows[4] {8, 7, 7, 7};
    int second_frame_cols[4] {4, 3, 4, 5};
    int normal_frame_rows[4] {piece_settings::current_row + 1, piece_settings::current_row,
                              piece_settings::current_row, piece_settings::current_row};
    int normal_frame_cols[4] {4, 3, 4, 5};
    size_t normal_frame_size = sizeof(normal_frame_rows) / sizeof(normal_frame_rows[0]);

    if (time_to_switch_frame())
    {
        if (piece_settings::current_frame == piece_settings::first_frame)
        {

            if (check_Ledplacement(8, 4))
            {
                return -1;
            }

            call_frame(first_frame_rows, first_frame_cols,
                       sizeof(first_frame_rows) / sizeof(first_frame_rows[0]), color::green);

            piece_settings::current_frame = piece_settings::second_frame;
            Time::last_frame_time         = Time::current_time;
        }
        else if (piece_settings::current_frame == piece_settings::second_frame)
        {
            if (check_Ledplacement(8 - 1, 4))
            {
                return -1;
            }
            // clear first frame here
            clear_frame(first_frame_rows, first_frame_cols,
                        sizeof(first_frame_rows) / sizeof(first_frame_rows[0]));
            // clear pixels on display
            clear_all_pixels();
            // set new leds
            call_frame(second_frame_rows, second_frame_cols,
                       sizeof(second_frame_rows) / sizeof(second_frame_rows[0]), color::green);

            piece_settings::current_frame = piece_settings::normal_frame;
            Time::last_frame_time         = Time::current_time;
        }
        else
        {
            if (piece_settings::current_row >= 0)
            {

                if (check_Ledplacement(piece_settings::current_row, 4) ||
                    check_Ledplacement(piece_settings::current_row, 5) ||
                    check_Ledplacement(piece_settings::current_row, 3))
                {
                    return -1;
                }
                else
                {
                    // clear second frame here
                    clear_frame(second_frame_rows, second_frame_cols,
                                sizeof(second_frame_rows) / sizeof(second_frame_rows[0]));
                    // move one row down till it reaches first row
                    // clear pixels
                    clear_all_pixels();
                    // printf("hey\n");
                    call_frame(normal_frame_rows, normal_frame_cols,
                               sizeof(normal_frame_rows) / sizeof(normal_frame_rows[0]),
                               color::green);
                    // check that the current row is above 1 and nothing is in
                    // its next row path, to clear ledmemory
                    if (piece_settings::current_row > 1 &&
                        !(check_Ledplacement(piece_settings::current_row - 1, 4) ||
                          check_Ledplacement(piece_settings::current_row - 1, 5) ||
                          check_Ledplacement(piece_settings::current_row - 1, 3)))
                    {
                        clear_frame(normal_frame_rows, normal_frame_cols,
                                    sizeof(normal_frame_rows) / sizeof(normal_frame_rows[0]));
                    }
                    --piece_settings::current_row;
                    Time::last_frame_time = Time::current_time;
                }
            }
        }
    }
    return piece_settings::current_row;
}

// could probably refactor this a bit..
int piece2()
{
    // 4 dots: 4 rows, one column
    Time::current_time = to_ms_since_boot(get_absolute_time());
    static bool first_frame {true};

    static int i {8};

    if (first_frame)
    {
        if (time_to_switch_frame())
        {
            if (i >= 5)
            {
                storeLed(i, 4, 0b00001101, 0b00001101, 00000000);
                sendLed();
                if (check_Ledplacement(i - 1, 4))
                {
                    return -1;
                }
                --i;
                Time::last_frame_time = Time::current_time;
            }
        }

        // clear memory
        if (i == 5)
        {
            first_frame = !first_frame;
        }
    }
    // already the next placement because of the j-- at the end of the loop
    else if (check_Ledplacement(i, 4) || i == 1)
    {
        return -1;
    }
    else if (time_to_switch_frame())
    {
        if (i >= 0)
        {
            // move one row down till it reaches first row
            // clear pixels
            clear_all_pixels();

            storeLed(i, 4, 0b00001101, 0b00001101, 00000000);
            clear_Ledmemory(i + 4, 4);
            sendLed();

            --i;
            Time::last_frame_time = Time::current_time;
        }
    }
    return i;
}

bool check_Ledplacement(int row, int col) { return (led_memory[row - 1][col - 1] > 0); }

bool game_end()
{
    bool game_off {true};
    for (int col = 0; col < 8; ++col)
    {
        for (int row = 0; row < 8; ++row)
        {
            if (led_memory[row][col] == 0)
            {
                game_off = false;
            }
        }
    }
    printf("game_off: %d\n", game_off);
    return game_off;
}

// todo:
// 3. make a function for pieces, that will work with multiple pieces, so
// avoiding repeating code
// 4. retain memoery of other piece when they stop moving
// 5. retain memory of the piece when it stopped moving and then reset the
// piece, so it can start its frames over.
