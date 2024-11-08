#include "led_control.h"
#include "pico/stdlib.h"
#include "led_memory.h"
#include "pwm_utils.h"

void send_bit_0(int pin)
{
    // gpio_put(pin, 1);
    // wait_0_4us();

    // gpio_put(pin, 0);
    // wait_0_85us();
    // cycle period = 0.4 us + 0.85 us = 1.25
    // wrap = 125 MHz * 1.25 us = 156
    // duty cycle = (0.4 µs / 1.25 µs) * wrap = 50
    update_pwm(pin, 156, 50, 1);
    // gpio_put(pin, 0);
}

void send_bit_1(int pin)
{
    // gpio_put(pin, 1);
    // wait_0_8us();

    // gpio_put(pin, 0);
    // wait_0_45us();
    // cycle period = 0.8 us + 0.45 us = 1.25
    // wrap = 125 MHz * 1.25 us = 156
    // duty cycle = (0.8 µs / 1.25 µs) * wrap = 100
    update_pwm(pin, 156, 100, 1);
    // gpio_put(pin, 0);
}
// Reset after sending all the color data
void reset_code()
{
    // above 50us to reset, does not need to be precise
    busy_wait_us(100);
}

// // clock running up to 133 MHz -- 1/ 133MHz = 7.52 nanoseconds

// inline void busy_wait_cycles(uint32_t cycles) 
// {
//     // A simple loop to waste clock cycles
//     // Adjusted loop to compensate for overhead
//     uint32_t adjusted_cycles = cycles / 11;  // Each iteration takes ~11 cycles
//     for (volatile uint32_t i = 0; i < adjusted_cycles; ++i) {
//         // No operation, just waste cycles
//         __asm volatile ("nop");
//     }
// }

// // Busy-wait for approximately 0.4 µs (400 ns)
// inline void wait_0_4us() 
// {
//     // 0.4 µs requires about 53 clock cycles at 133 MHz
//     busy_wait_cycles(50);
// }
// // running at 125 MHz, 8 ns/clock
// // Busy-wait for approximately 0.45 µs (450 ns)
// // 450 ns / 7.52 ns/clock
// inline void wait_0_45us() 
// {
//     // 0.45 µs requires about 59 clock cycles at 133 MHz
//     busy_wait_cycles(56);
// }

// // Busy-wait for approximately 0.8 µs (800 ns)
// inline void wait_0_8us() 
// {
//     // 0.8 µs requires about 106 clock cycles at 133 MHz
//     busy_wait_cycles(100);
// }

// // Busy-wait for approximately 0.85 µs (850 ns)
// inline void wait_0_85us() 
// {
//     // 0.85 µs requires about 113 clock cycles at 133 MHz
//     // turning it a bit down, it is more slow
//     busy_wait_cycles(106);
// }

// colors
void send_color_bits(std::uint8_t color, int pin)
{
    for (int i = 7; i >= 0; --i)
    {
        if (color & (1 << i))
        {
            send_bit_1(pin);
        }
        else
        {
            send_bit_0(pin);
        }
        
    }
}

void send_grb(std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin)
{
    send_color_bits(green, pin);
    send_color_bits(red,   pin);
    send_color_bits(blue,  pin);
}
// set color by row and col
void setLedColor(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin)
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
        send_grb(0b00000000, 0b00000000, 0b00000000, pin);
    }
    send_grb(green, red, blue, pin);
    
}

void storeLed(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin)
{
    // packing values into 32 bits
    std::uint32_t grb = static_cast<std::uint32_t>((green << 16) | (red << 8) | blue);

    // assign the color to the position
    led_memory[row - 1][col - 1] = grb;
    
}


// loops through the led_memory in correct order, and send the ledcolors
void sendLed(int pin)
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
                    setLedColor(i + 1, j + 1, green, red, blue, pin);
                    reset_code();
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
                    setLedColor(i + 1, j + 1, green, red, blue, pin);
                    reset_code();
                }
            }
        }
        
    }
}