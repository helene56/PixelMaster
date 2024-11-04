#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include <iostream>
#include "led_control.h"
#include "led_memory.h"
#include "animations.h"

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

    // storeLed(5, 2,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(5, 3,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 2,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 3,  0b0101, 0b0101, 0b0101, Pins::DIN);

    // storeLed(5, 6,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(5, 7,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 6,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 7,  0b0101, 0b0101, 0b0101, Pins::DIN);

    // storeLed(5, 2,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 3,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(7, 2,  0b0101, 0b0101, 0b0101, Pins::DIN);

    // storeLed(5, 7,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(6, 6,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(7, 7,  0b0101, 0b0101, 0b0101, Pins::DIN);


    // storeLed(3, 3,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(2, 4,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(2, 5,  0b0101, 0b0101, 0b0101, Pins::DIN);
    // storeLed(1, 1,  0b0101, 0b0101, 0b0101, Pins::DIN)
    
    // sendLed(Pins::DIN);
    // need to reset first color for some reason
    setLedColor(1, 1, 0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    setLedColor(1, 1, 0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    reset_code();
    
    while (true) 
    {
        // run(Pins::DIN);
        face(Pins::DIN);
        
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

