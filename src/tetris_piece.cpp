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
    static int ran_num {random_generator(seed, 5)};
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

bool time_to_switch_frame()
{
    return (Time::current_time - Time::last_frame_time >= Time::interval);
}

int random_generator(std::uint16_t X, int range)
{
    X ^= (X << 13);  // XOR X with itself shifted left by 13 bits
    X ^= (X >> 17);  // XOR result with itself shifted right by 17 bits
    X ^= (X << 5);   // XOR result with itself shifted left by 5 bits

    return X % range; // range of num 0-4
}



void generate_piece()
{
    printf("random number: %d\n", random::ran_num);
    
    switch (random::ran_num)
    {
    case 0:
        piece1();
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    
    default:
        break;
    }
}


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
    static bool first_frame {true};
    static bool second_frame {false};
    static int i {6};

    int first_frame_rows[3] {8, 8, 8};
    int first_frame_cols[3] {3, 4, 5};

    if (time_to_switch_frame())
    {
        if (first_frame)
        {

            // storeLed(8, 3, 0b00001101, 0b00000000, 00000000);
            // storeLed(8, 4, 0b00001101, 0b00000000, 00000000);
            // storeLed(8, 5, 0b00001101, 0b00000000, 00000000);
            // sendLed();
            call_frame(first_frame_rows, first_frame_cols, sizeof(first_frame_rows) / sizeof(first_frame_rows[0]), color::green);
            if (check_Ledplacement(8-1, 4))
            {
                return -1;
            }
            first_frame = false;
            second_frame = true;
            Time::last_frame_time = Time::current_time;

            // clear memory
            // clear_Ledmemory(8, 3);
            // clear_Ledmemory(8, 4);
            // clear_Ledmemory(8, 5);
            clear_frame(first_frame_rows, first_frame_cols, sizeof(first_frame_rows) / sizeof(first_frame_rows[0]));
        }
        else if (second_frame)
        {
            // clear pixels on display
            clear_all_pixels();
            // set new leds
            storeLed(8, 4, 0b00001101, 0b00000000, 00000000);
            storeLed(7, 3, 0b00001101, 0b00000000, 00000000);
            storeLed(7, 4, 0b00001101, 0b00000000, 00000000);
            storeLed(7, 5, 0b00001101, 0b00000000, 00000000);
            sendLed();
            if (check_Ledplacement(7-1, 4))
            {
                return -1;
            }
            second_frame = false;
            Time::last_frame_time = Time::current_time;
            
            clear_Ledmemory(8, 4);
            clear_Ledmemory(7, 3);
            clear_Ledmemory(7, 4);
            clear_Ledmemory(7, 5);   
        }
        else
        {
            if (i >= 0)
            {
                if (check_Ledplacement(i, 4) || check_Ledplacement(i, 5) || check_Ledplacement(i, 3))
                {
                    return -1;
                }
                else
                {
                    // move one row down till it reaches first row
                    // clear pixels
                    clear_all_pixels();
                    storeLed(i+1, 4, 0b00001101, 0b00000000, 00000000);
                    storeLed(i, 3, 0b00001101, 0b00000000, 00000000);
                    storeLed(i, 4, 0b00001101, 0b00000000, 00000000);
                    storeLed(i, 5, 0b00001101, 0b00000000, 00000000);
                    sendLed();

                    if (i > 1)
                    {
                        clear_Ledmemory(i+1, 4);
                        clear_Ledmemory(i, 3);
                        clear_Ledmemory(i, 4);
                        clear_Ledmemory(i, 5);
                    }
                    --i;
                    Time::last_frame_time = Time::current_time;
                }

            }
        }
    }
    return i;
}


void piece2()
{
    // 4 dots: 4 rows, one column
    Time::current_time = to_ms_since_boot(get_absolute_time());
    static bool first_frame {true};

    static int i {8};
    static int j {7};

    // for now col is hardcoded
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
                    return;
                }
                --i;
                Time::last_frame_time = Time::current_time;
            }
            
        }
        
        // clear memory
        if (i == 5)
        {
            first_frame = !first_frame;
            clear_Ledmemory(8, 4);
            clear_Ledmemory(7, 4);
            clear_Ledmemory(6, 4);
            clear_Ledmemory(5, 4);
        }
    }
    // already the next placement because of the j-- at the end of the loop
    else if (check_Ledplacement(j-3, 4))
    {
        return;
    }
    else if (time_to_switch_frame())
    {
        if (j >= 4)
        {
            // move one row down till it reaches first row
            // clear pixels
            clear_all_pixels();
            storeLed(j, 4, 0b00001101, 0b00001101, 00000000);
            storeLed(j-1, 4, 0b00001101, 0b00001101, 00000000);
            storeLed(j-2, 4, 0b00001101, 0b00001101, 00000000);
            storeLed(j-3, 4, 0b00001101, 0b00001101, 00000000);
            sendLed();
            if (j > 4)
            {
                clear_Ledmemory(j, 4);
                clear_Ledmemory(j-1, 4);
                clear_Ledmemory(j-2, 4);
                clear_Ledmemory(j-3, 4);
            }
            --j;
            Time::last_frame_time = Time::current_time;
        }
        
    }
    
}


bool check_Ledplacement(int row, int col)
{

    return (led_memory[row - 1][col - 1] > 0);
}

// todo:
// 1. make a way to store the different frames for the pieces, 
// for easy clear_Ledmemory access, just to clean code up a bit
// 2. add a function to check if row col is already taken up by one piece, 
// if so, the other piece should stop moving
// 3. apply 2. to the beginning frames of the piece as well and add to piece1 as well

