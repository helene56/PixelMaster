#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include <iostream>
#include "led_control.h"
#include "led_memory.h"
#include "animations.h"
#include "pio.h"

namespace Pins
{
    // Data In pin
    constexpr int DIN {16};
} // namespace Pins

int main()
{
    stdio_init_all();
    gpio_init(Pins::DIN);
    // gpio_set_dir(Pins::DIN, GPIO_OUT);

    // add small delay, when powered by usb microcontroller, 
    // otherwise initializing is not correct
    busy_wait_us(500000); // 500 ms
    // and clear first led.. there must be some disturbance
    setLedColor(1, 1, 0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    
    // setup pin for pio machine
    set_gpio(Pins::DIN);
    // setup program
    setup_pio();
    while (true) 
    {
        // test pio machine
        load_color(0b00000000, 0b00000000, 0b11111111);
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

