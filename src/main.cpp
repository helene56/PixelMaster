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
    // gpio_init(Pins::DIN);
    // gpio_set_dir(Pins::DIN, GPIO_OUT);

    // add small delay, when powered by usb microcontroller, 
    // otherwise initializing is not correct
    // busy_wait_us(500000); // 500 ms

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &wave_program);
    uint clock = clock_get_hz(clk_sys);
    wave_program_init(pio, sm, offset, Pins::DIN, 800000, clock);

    
    while (true) 
    {
        
        // test pio machine

        put_pixel(urgb_u32(0b11111111, 0b11111111, 0b00000000));
        sleep_ms(1000);


        
        // put_pixel(urgb_u32(0b11111111, 0b11111111, 0b00000000));
        // reset_code();
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        
    }
}

