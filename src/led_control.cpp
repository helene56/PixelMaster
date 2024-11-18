#include "led_control.h"
#include "pico/stdlib.h"
#include "led_memory.h"
#include "pio_utils.h"
#include <stdio.h>


// set color by row and col
void setLedColor(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue)
{
    int move {0};
    if (row % 2 == 0)
    {
        move = (row * 8) - col;
    }
    else if (row == 1)
    {
        move = col - 1;
    }
    else
    {
        move = (row * 8) - (8 - col) - 1;
    }
    
    for (int i = 1; i <= move; ++i)
    {
        put_pixel(ugrb_u32(0b00000000, 0b00000000, 0b00000000));
    }
    put_pixel(ugrb_u32(green, red, blue));
    
}

void storeLed(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue)
{
    // packing values into 32 bits
    std::uint32_t grb = static_cast<std::uint32_t>((green << 16) | (red << 8) | blue);

    // assign the color to the position
    led_memory[row - 1][col - 1] = grb;
    
}


// loops through the led_memory in correct order, and send the ledcolors
void sendLed()
{
    // look through led_memory and send all
    for (int i = 7; i >= 0; --i)
    {

        if (i % 2 != 0)
        {
            for (int j = 0; j <= 7; ++j)
            {
                if (led_memory[i][j] != 0)
                {
                    std::uint8_t green = (led_memory[i][j] >> 16) & 0xFF;  // Extract the red component (8 bits)
                    std::uint8_t red = (led_memory[i][j] >> 8) & 0xFF; // Extract the green component (8 bits)
                    std::uint8_t blue = led_memory[i][j] & 0xFF;         // Extract the blue component (8 bits)
                    setLedColor(i + 1, j + 1, green, red, blue);
                    reset_pixel();
                }
            }
        }
        else
        {
            for (int j = 7; j >= 0; --j)
            {
                if (led_memory[i][j] != 0)
                {
                    std::uint8_t green = (led_memory[i][j] >> 16) & 0xFF;  // Extract the red component (8 bits)
                    std::uint8_t red = (led_memory[i][j] >> 8) & 0xFF; // Extract the green component (8 bits)
                    std::uint8_t blue = led_memory[i][j] & 0xFF;         // Extract the blue component (8 bits)
                    setLedColor(i + 1, j + 1, green, red, blue);
                    reset_pixel();
                }
            }
        }
        
    }
}