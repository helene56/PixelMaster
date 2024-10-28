#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include "led_control.h"

namespace Pins
{
    constexpr int PIN16 {16};
} // namespace Pins

int main()
{
    stdio_init_all();
    gpio_init(Pins::PIN16);
    gpio_set_dir(Pins::PIN16, GPIO_OUT);
    // set_sys_clock_khz(133000, true);  // Set the clock to 133 MHz
    // for some reason first led will not properly work, if another signal hasn't been sent.
    send_grb(0b00000000, 0b00000000, 0b00000000, Pins::PIN16);
    reset_code();

    send_grb(0b00000000, 0b11111111, 0b00000000, Pins::PIN16);
    send_grb(0b10010110, 0b00000000, 0b00000000, Pins::PIN16);
    send_grb(0b00000000, 0b00000000, 0b11111111, Pins::PIN16);
    send_grb(0b00000000, 0b11111111, 0b00000000, Pins::PIN16);
    send_grb(0b01101100, 0b11000110, 0b00111000, Pins::PIN16);
    reset_code();
    send_grb(0b10010110, 0b00000000, 0b00000000, Pins::PIN16);
    // send_grb(0b00000000, 0b11111111, 0b00000000);
    while (true) {
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

