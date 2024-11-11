#include "hardware/pio.h"
#include "pico/stdlib.h"
#include "pio_machine.h"
#include <cstdint>

#include "build/wave.pio.h"


void set_gpio(int pin)
{
    pio_gpio_init(pio0, pin);
    pio_sm_set_consecutive_pindirs(pio0, statemachine::sm, pin, 1, true);
}

void setup_pio()
{
    pio_add_program(pio0, &wave_program);
    // start the program
    pio_sm_set_enabled(pio0, statemachine::sm, true);
    // configure clock dividers: if set at 1, it runs at full speed of 125 MHz, 
    // if set at 2, it runs at half the speed 62.5 MHz, (each PIO cycle becomes 16 nanoseconds)
    // tip: Suppose you need each nop instruction to represent 8 microseconds. 
    // You could set the clock divider to 1000 (125 MHz / 1000 = 125 kHz), 
    // making each cycle 8 microseconds long.
    pio_sm_set_clkdiv(pio0, statemachine::sm, 1);
}

void load_color(std::uint8_t green, std::uint8_t red, std::uint8_t blue)
{
    pio_sm_put_blocking(pio0, statemachine::sm, green);
    pio_sm_put_blocking(pio0, statemachine::sm, red);
    pio_sm_put_blocking(pio0, statemachine::sm, blue);
}