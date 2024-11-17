#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include <iostream>
#include "led_control.h"
#include "led_memory.h"
#include "animations.h"

#include "build/wave.pio.h"

#include "pio_utils.h"

namespace Pins
{
    // Data In pin
    constexpr int DIN {16};
} // namespace Pins

int main()
{
    stdio_init_all();
    
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &wave_program);
    uint clock = clock_get_hz(clk_sys);
    wave_program_init(pio, sm, offset, Pins::DIN, 800000, clock);
    
    while (true) 
    {
        
        
        // test pio machine
        face();
        // storeLed(1, 3, 0b00, 0b1010, 0b00);
        // storeLed(1, 2, 0b00, 0b1010, 0b00);
        // storeLed(1, 4, 0b00, 0b1010, 0b00);
        // storeLed(2, 4, 0b00, 0b1010, 0b00);

        // storeLed(8, 2, 0b00, 0b1010, 0b00);
        // sendLed();
        // sleep_ms(1000);
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        
    }
}

