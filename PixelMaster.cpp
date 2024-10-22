#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <cstdint>

constexpr int PIN16 {16};

// function prototypes
// loop to waste clock cycles, to have precise control over the wait time
inline void busy_wait_cycles(uint32_t cycles);
inline void wait_0_4us();
inline void wait_0_45us();
inline void wait_0_8us();
inline void wait_0_85us();
// color functions

// helper function to send 8 color bits
void send_color_bits(std::uint8_t color);
// send a 24 bits grb code to a led
void send_grb( std::uint8_t green, std::uint8_t red, std::uint8_t blue);
// Reset after sending all the color data
void reset_code();
// custom function to set pin high on hardware level to reduce overhead time
inline void set_high(int pin);
// custom function to set pin low on hardware level to reduce overhead time
inline void set_low(int pin);
// send bit 0
void send_bit_0();
// send bit 1
void send_bit_1();

int main()
{
    stdio_init_all();
    gpio_init(PIN16);
    gpio_set_dir(PIN16, GPIO_OUT);
    

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void send_bit_0()
{
    // T0H();
    set_high(PIN16);
    wait_0_4us();
    // T0L();
    set_low(PIN16);
    wait_0_85us();
}

void send_bit_1()
{
    // T1H();
    set_high(PIN16);
    wait_0_8us();
    // T1L();
    set_low(PIN16);
    wait_0_45us();
}
// Reset after sending all the color data
void reset_code()
{
    // above 50us to reset, does not need to be precise
    busy_wait_us(100);
}

inline void set_high(int pin)
{
    sio_hw->gpio_set = (1 << pin);
}
inline void set_low(int pin)
{
    sio_hw->gpio_clr = (1 << pin);
}

// clock running up to 133 MHz -- 1/ 133MHz = 7.52 nanoseconds

inline void busy_wait_cycles(uint32_t cycles) 
{
    // A simple loop to waste clock cycles
    for (volatile uint32_t i = 0; i < cycles; i++) {
        // No operation, just waste cycles
        __asm volatile ("nop");
    }
}

// Busy-wait for approximately 0.4 µs (400 ns)
inline void wait_0_4us() 
{
    // 0.4 µs requires about 53 clock cycles at 133 MHz
    busy_wait_cycles(53);
}

// Busy-wait for approximately 0.45 µs (450 ns)
// 450 ns / 7.52 ns/clock
inline void wait_0_45us() 
{
    // 0.45 µs requires about 59 clock cycles at 133 MHz
    busy_wait_cycles(59);
}

// Busy-wait for approximately 0.8 µs (800 ns)
inline void wait_0_8us() 
{
    // 0.8 µs requires about 106 clock cycles at 133 MHz
    busy_wait_cycles(106);
}

// Busy-wait for approximately 0.85 µs (850 ns)
inline void wait_0_85us() 
{
    // 0.85 µs requires about 113 clock cycles at 133 MHz
    busy_wait_cycles(113);
}

// colors
// TODO: need to rework this
void green()
{
    // send 10010110'00000000'00000000 for green
    send_bit_1();
    send_bit_0();
    send_bit_0();
    send_bit_1();
    send_bit_0();
    send_bit_1();
    send_bit_1();
    send_bit_0();

    for (volatile int i = 0; i <16; ++i)
    {
        send_bit_0();
    }

}
// send 00000000'10010110'00000000 for green in medium brigthness
void send_color_bits(std::uint8_t color)
{
    for (int i = 7; i >= 0; --i)
    {
        if (color & (1 << i))
        {
            send_bit_1();
        }
        else
        {
            send_bit_0();
        }
        
    }
}

void send_grb( std::uint8_t green, std::uint8_t red, std::uint8_t blue)
{
    send_color_bits(green);
    send_color_bits(red);
    send_color_bits(blue);
}


