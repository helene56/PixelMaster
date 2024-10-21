#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


constexpr int PIN16 {16};

// function prototypes
// loop to waste clock cycles, to have precise control over the wait time
inline void busy_wait_cycles(uint32_t cycles);
inline void wait_0_4us();
inline void wait_0_45us();
inline void wait_0_8us();
inline void wait_0_85us();

inline void set_high(int pin);
inline void set_low(int pin);
// transfer 0 high
void T0H();
// transfer 1 high
void T1H();
// transfer 0 low
void T0L();
// transfer 1 low
void T1L();


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

void T0H()
{
    set_high(PIN16);
    wait_0_4us();
}

void T1H()
{
    set_high(PIN16);
    wait_0_8us();
}

void T0L()
{
    set_low(PIN16);
    wait_0_85us();

}

void T1L()
{
    set_low(PIN16);
    wait_0_45us();
}

void code0()
{
    T0H();
    T0L();
}

void code1()
{
    T1H();
    T1L();
}

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

// Busy-wait for approximately 0.8 µs (850 ns)
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


