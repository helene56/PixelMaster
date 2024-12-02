#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>

#include <iostream>
#include "led_control.h"
#include "led_memory.h"
#include "animations.h"
#include "tetris_piece.h"

#include "build/wave.pio.h"

#include "pio_utils.h"

namespace Pins
{
    // Data In pin
    constexpr int DIN {16};
    constexpr int button {15};
    constexpr int light {14};
    constexpr int read_light {13};
} // namespace Pins


namespace frames
{
    uint32_t last_frame_time = 0;
    uint32_t FRAME_INTERVAL = 500;
    volatile bool button_pressed = false;
    bool is_face_1 = true; // Tracks which face is currently displayed
} // namespace frames

int main()
{
    stdio_init_all();
    // initialize button
    gpio_init(Pins::button);
    gpio_set_dir(Pins::button, GPIO_IN);
    gpio_pull_up(Pins::button);

    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &wave_program);
    // uint clock = clock_get_hz(clk_sys);
    wave_program_init(pio, sm, offset, Pins::DIN, 800000);
    

    std::uint32_t w {0x050505};
    std::uint32_t y {0x030301}; // yellow at 5% brightness
    
    // storeLed(8, 4, 0b00001101, 0b00001101, 00000000);
    // storeLed(7, 3, 0b00001101, 0b00001101, 00000000);
    // storeLed(7, 4, 0b00001101, 0b00001101, 00000000);
    // storeLed(7, 5, 0b00001101, 0b00001101, 00000000);
    // sendLed();
    // sleep_ms(7000);
    
    storeLed(6, 4, 0b00001101, 0b00000000, 00000000);
    sendLed();

    while (true) 
    {

        // uint32_t current_time = to_ms_since_boot(get_absolute_time());
        // // Check if it's time to switch frames
        // if (current_time - frames::last_frame_time >= frames::FRAME_INTERVAL) 
        // {
            
        // }
        // piece1();
        // if (piece1() <= 0)
        // {
        //     piece2();
        // }
        piece1();
        
        // small delay for button to be read correctly
        sleep_ms(2);
         
    }
}

