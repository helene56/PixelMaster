#include "pico/stdlib.h"
#include "led_control.h"
#include "led_memory.h"
#include "pio_utils.h"
#include <stdio.h>

// makes an led go through all positions in various colors and framerates
void run()
{
    std::uint8_t greens[3] {0b00000000, 0b1010, 0b00000000};
    std::uint8_t blues[3] {0b1010, 0b00000000, 0b00000000};
    std::uint8_t reds[3] {0b00000000, 0b00000000, 0b1010};
    uint sleeps[3] {50, 60, 55};
    for (int l = 0; l < 3; ++l)
    
    {
        for (int i = 1; i <= 8; ++i)
        {
            // clear all leds
            for (int k = 0; k < 64; ++k)
            {
                put_pixel(ugrb_u32(0b00000000, 0b00000000, 0b00000000));
            }
            for (int j = 1; j <= 8; ++j)
            {
                
                setLedColor(i, j, greens[l], reds[l], blues[l]);
                sleep_ms(sleeps[l]);
                // sleep_ms(sleeps[l]);
                // reset_pixel();
                

            }
        }
    }
}

void face()
{
    std::uint32_t w {0x050505};
    std::uint32_t face1[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, w, w, 0, 0, w, w, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};

    std::uint32_t face2[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, w, 0, 0, 0, 0, w, 0,
                               0, 0, w, 0, 0, w, 0, 0,
                               0, w, 0, 0, 0, 0, w, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};



    std::uint32_t (*faces[2])[8] = {face1, face2};

    for (int k = 0; k < 2; ++k)
    {
         std::uint32_t* ptr = &faces[k][0][0];  

        for (int i = 0; i < 64; ++i) 
        {  // Iterate over 64 elements
            // Use *ptr to access the current element
            std::uint32_t value = *ptr;



            // Calculate row and column for led_memory in reverse order
            int row = (i / 8);  // Reverse the row index
            int col = (i % 8);  // Reverse the column index

            // Store the value in led_memory
            led_memory[row][col] = value;

            // Move the pointer backward
            ++ptr;
        }
        sendLed();
        sleep_ms(500);
    } 
}
