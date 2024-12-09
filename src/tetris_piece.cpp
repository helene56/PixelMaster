#include <stdio.h>
#include "led_control.h"
#include "tetris_piece.h"
#include "pico/stdlib.h"
#include "pio_utils.h"
#include "led_memory.h"
// steps
// generate random piece at random location, start in the top of the matrix
// move it down till it reaches the bottom

namespace random
{
    std::uint16_t seed {42};
    int ran_num {random_generator(seed, 5)};
} // namespace random


namespace color
{
    std::uint32_t green = (0b00001101 << 16) | (0b00000000 << 8) | 0b00000000;

} // namespace color


namespace Time
{
    uint32_t interval = 800;
    static uint32_t last_frame_time = 0;
    static uint32_t current_time = to_ms_since_boot(get_absolute_time());
    //  frames::last_frame_time = current_time;
    bool switch_frame = (current_time - last_frame_time >= interval);
} // namespace Time


namespace piece_settings
{
    enum Framestate {first_frame, second_frame, normal_frame};
    static Framestate current_frame {first_frame};
    static int current_row {6};
} // namespace piece_settings


bool time_to_switch_frame()
{
    return (Time::current_time - Time::last_frame_time >= Time::interval);
}

int random_generator(std::uint16_t &X, int range)
{
    X ^= (X << 13);  // XOR X with itself shifted left by 13 bits
    X ^= (X >> 17);  // XOR result with itself shifted right by 17 bits
    X ^= (X << 5);   // XOR result with itself shifted left by 5 bits

    int result = X % range;
    X ^= (X >> 5);  // Update seed to prepare for next call
    // return X % range; // range of num 0-4
    return result;
}



void generate_piece()
{
    static int result {1};
    // temp count otherwise the led will reset it self indefinitely. 
    // needs some kind of look up to check if it should be refreshed or not..
    // pieces have different length so only works with piece1
    // static int count {0};
    printf("ledmemory[8][4] = %d\n", led_memory[7][3]);
    if (result <= 0 && !game_end())
    {
        // the piece that just ran should be reset now, but the last leds should remain
        piece_settings::current_frame = piece_settings::first_frame;
        piece_settings::current_row = 6;
        random::ran_num = random_generator(random::seed, 5);
        result = 1;
        // count++;
    }
    // if (piece_settings::first_frame)
    // {
    //     printf("first frame\n");
    // }
    // else if (piece_settings::second_frame)
    // {
    //     printf("second frame\n");
    // }
    
    
    switch (random::ran_num)
    {
    case 0:
        result = piece1();
        break;
    case 1:
        result = piece1();
        break;
    case 2:
        result = piece1();
        break;
    case 3:
        result = piece1();
        break;
    case 4:
        result = piece1();
        break;
    
    default:
        break;
    }
}

// refactor these two functions under here somehow, they almost do the same
void call_frame(int *rows, int *cols, int size, std::int32_t grb)
{
    int *p = rows;
    int *p2 = cols;
    std::uint8_t green = (grb >> 16) & 0xFF;  // Extract the red component (8 bits)
    std::uint8_t red = (grb >> 8) & 0xFF; // Extract the green component (8 bits)
    std::uint8_t blue = grb & 0xFF;         // Extract the blue component (8 bits)

    for (int i = 0; i < size; ++i)
    {  
        storeLed(*p, *p2, green, red, blue);
        ++p;
        ++p2;
    }
    sendLed();
        

}

void store_latestLed(int *rows, int *cols, int size, std::int32_t grb)
{
    std::uint8_t green = (grb >> 16) & 0xFF;  // Extract the red component (8 bits)
    std::uint8_t red = (grb >> 8) & 0xFF; // Extract the green component (8 bits)
    std::uint8_t blue = grb & 0xFF;         // Extract the blue component (8 bits)
    int *p = rows;
    int *p2 = cols;
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
    int *p = rows;
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
    int normal_frame_rows[4] {piece_settings::current_row+1, piece_settings::current_row, piece_settings::current_row, piece_settings::current_row};
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
            Time::last_frame_time = Time::current_time;
            
        }
        else if (piece_settings::current_frame == piece_settings::second_frame)
        {
            if (check_Ledplacement(8-1, 4))
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
            Time::last_frame_time = Time::current_time;
               
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
                    sizeof(normal_frame_rows) / sizeof(normal_frame_rows[0]), color::green);
                    // check that the current row is above 1 and nothing is in its next row path, to clear ledmemory
                    if (piece_settings::current_row > 1 && 
                    !(check_Ledplacement(piece_settings::current_row-1, 4) || check_Ledplacement(piece_settings::current_row-1, 5) || check_Ledplacement(piece_settings::current_row-1, 3)))
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
                if (check_Ledplacement(i-1, 4))
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
            clear_Ledmemory(i+4, 4);
            sendLed();

            --i;
            Time::last_frame_time = Time::current_time;
        }
        
    }
    return i;
    
}


bool check_Ledplacement(int row, int col)
{

    return (led_memory[row - 1][col - 1] > 0);
}

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
// 4. retain memoery of other piece when they stop moving
// 5. retain memory of the piece when it stopped moving and then reset the piece, so it can start its frames over.
