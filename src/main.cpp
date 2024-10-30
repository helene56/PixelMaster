#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include "led_control.h"

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
    // for some reason first led will not properly work, if another signal hasn't been sent.
    send_grb(0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();

    setLedColor(5, 2, 0b1010, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    setLedColor(2, 5, 0b1010, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    setLedColor(2, 6, 0b1010, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    setLedColor(2, 7, 0b1010, 0b00000000, 0b00000000, Pins::DIN);
    // send_grb(0b00000000, 0b11111111, 0b00000000);
    while (true) {
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

