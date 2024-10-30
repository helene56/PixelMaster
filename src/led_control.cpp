#include "led_control.h"
#include "pico/stdlib.h"


void send_bit_0(int pin)
{
    gpio_put(pin, 1);
    wait_0_4us();

    gpio_put(pin, 0);
    wait_0_85us();
}

void send_bit_1(int pin)
{
    gpio_put(pin, 1);
    wait_0_8us();

    gpio_put(pin, 0);
    wait_0_45us();
}
// Reset after sending all the color data
void reset_code()
{
    // above 50us to reset, does not need to be precise
    busy_wait_us(100);
}

// clock running up to 133 MHz -- 1/ 133MHz = 7.52 nanoseconds

inline void busy_wait_cycles(uint32_t cycles) 
{
    // A simple loop to waste clock cycles
    // Adjusted loop to compensate for overhead
    uint32_t adjusted_cycles = cycles / 11;  // Each iteration takes ~11 cycles
    for (volatile uint32_t i = 0; i < adjusted_cycles; ++i) {
        // No operation, just waste cycles
        __asm volatile ("nop");
    }
}

// Busy-wait for approximately 0.4 µs (400 ns)
inline void wait_0_4us() 
{
    // 0.4 µs requires about 53 clock cycles at 133 MHz
    busy_wait_cycles(50);
}
// running at 125 MHz, 8 ns/clock
// Busy-wait for approximately 0.45 µs (450 ns)
// 450 ns / 7.52 ns/clock
inline void wait_0_45us() 
{
    // 0.45 µs requires about 59 clock cycles at 133 MHz
    busy_wait_cycles(56);
}

// Busy-wait for approximately 0.8 µs (800 ns)
inline void wait_0_8us() 
{
    // 0.8 µs requires about 106 clock cycles at 133 MHz
    busy_wait_cycles(100);
}

// Busy-wait for approximately 0.85 µs (850 ns)
inline void wait_0_85us() 
{
    // 0.85 µs requires about 113 clock cycles at 133 MHz
    // turning it a bit down, it is more slow
    busy_wait_cycles(106);
}

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