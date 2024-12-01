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

namespace Time
{
    uint32_t interval = 400;
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


void piece1()
{
    Time::current_time = to_ms_since_boot(get_absolute_time());
    static bool first_frame {true};

    // for now col is hardcoded
    if (first_frame)
    {
        storeLed(8, 3, 0b00001101, 0b00001101, 00000000);
        storeLed(8, 4, 0b00001101, 0b00001101, 00000000);
        storeLed(8, 5, 0b00001101, 0b00001101, 00000000);
        sendLed();
        // clear memory
        resetLedmemory();
    }
    
    static int i {6};
   
    if (i >= 0)
    {
        if (time_to_switch_frame())
        {
            
            if (first_frame)
            {
                // clear pixels on display
                for (int k = 0; k < 64; ++k)
                {
                    put_pixel(ugrb_u32(0b00000000, 0b00000000, 0b00000000));
                }
                reset_pixel();
                // set new leds
                storeLed(8, 4, 0b00001101, 0b00001101, 00000000);
                storeLed(7, 3, 0b00001101, 0b00001101, 00000000);
                storeLed(7, 4, 0b00001101, 0b00001101, 00000000);
                storeLed(7, 5, 0b00001101, 0b00001101, 00000000);
                sendLed();
                first_frame = false;
                Time::last_frame_time = Time::current_time;
            }
            else
            {
                // move one row down till it reaches first row
                // clear pixels
                for (int k = 0; k < 64; ++k)
                {
                    put_pixel(ugrb_u32(0b00000000, 0b00000000, 0b00000000));
                }
                reset_pixel();
                storeLed(i+1, 4, 0b00001101, 0b00001101, 00000000);
                storeLed(i, 3, 0b00001101, 0b00001101, 00000000);
                storeLed(i, 4, 0b00001101, 0b00001101, 00000000);
                storeLed(i, 5, 0b00001101, 0b00001101, 00000000);
                sendLed();
                
                --i;
                Time::last_frame_time = Time::current_time;
                
            
            }
            
            if (i > 0)
            {
                // clear memory
                resetLedmemory(); 
            }  

        }
    }
}

// todo:
// make function which only clear memory for moving piece, the pieces at the bottom, should still be displayed

