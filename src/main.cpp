#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include <iostream>
#include "led_control.h"
#include "led_memory.h"

namespace Pins
{
    // Data In pin
    constexpr int DIN {16};
} // namespace Pins

int main()
{
    stdio_init_all();
    gpio_init(Pins::DIN);
    gpio_set_dir(Pins::DIN, GPIO_OUT);

    // set_sys_clock_khz(133000, true);  // Set the clock to 133 MHz

    // add small delay, when powered by usb microcontroller, 
    // otherwise initializing is not correct
    busy_wait_us(500000); // 500 ms

    storeLed(2, 5,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    storeLed(2, 7,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    storeLed(2, 6,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    storeLed(6, 1,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    storeLed(7, 6,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    storeLed(8, 1,  0b1010, 0b00000000, 0b00000000, Pins::DIN);
    sendLed(Pins::DIN);

    while (true) {
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

