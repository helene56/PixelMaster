#include "pico/stdlib.h"
#include "led_control.h"

// makes an led go through all positions in various colors and framerates
void run(int pin)
{
    std::uint8_t greens[3] {0b1010, 0b00000000, 0b00000000};
    std::uint8_t blues[3] {0b00000000, 0b1010, 0b00000000};
    std::uint8_t reds[3] {0b00000000, 0b00000000, 0b1010};
    uint sleeps[3] {50, 60, 55};
    for (int l = 0; l < 3; ++l)
    {
        for (int i = 1; i <= 8; ++i)
        {
            for (int j = 1; j <= 8; ++j)
            {
                // clear all leds
                for (int k = 0; k < 64; ++k)
                {
                    send_grb(0b00000000, 0b00000000, 0b00000000, pin);
                }
                reset_code();
                setLedColor(i, j, greens[l], reds[l], blues[l], pin);
                reset_code();
                sleep_ms(sleeps[l]);

            }
        }
    }
}
