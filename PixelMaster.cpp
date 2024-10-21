#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"


constexpr int PIN16 {16};

// function prototypes

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
    busy_wait_us(0.4);
}

void T1H()
{
    set_high(PIN16);
    busy_wait_us(0.8);
}

void T0L()
{
    set_low(PIN16);
    busy_wait_us(0.85);

}

void T1L()
{
    set_low(PIN16);
    busy_wait_us(0.45);
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
    // above 50us to reset
    busy_wait_us(100);
}

inline void set_high(volatile int pin)
{
    sio_hw->gpio_set = (1 << pin);
}
inline void set_low(volatile int pin)
{
    sio_hw->gpio_clr = (1 << pin);
}
