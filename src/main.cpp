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
    constexpr int button {15};
    constexpr int light {14};
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

    // initialize light
    gpio_init(Pins::light);
    gpio_set_dir(Pins::light, GPIO_OUT);
    gpio_put(Pins::light, 1);
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &wave_program);
    uint clock = clock_get_hz(clk_sys);
    wave_program_init(pio, sm, offset, Pins::DIN, 800000, clock);
    

    std::uint32_t w {0x050505};
    std::uint32_t y {0x373F16};
    std::uint32_t neutral[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, w, w, 0, 0, w, w, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};

    std::uint32_t face2[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, w, 0, 0, 0, 0, w, 0,
                               0, 0, w, 0, 0, w, 0, 0,
                               0, w, 0, 0, 0, 0, w, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};

    std::uint32_t face3[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               w, w, 0, 0, w, w, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};

    std::uint32_t face4[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, w, w, 0, 0, w, w,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, w, 0, 0, w, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,};

    std::uint32_t happy[8][8] {0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, w, 0, 0, 0, 0, w, 0,
                               w, 0, w, 0, 0, w, 0, w,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, w, w, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,
                               0, 0, 0, 0, 0, 0, 0, 0,};
    
    std::uint32_t light_bulb[8][8] {0, 0, y, 0, 0, y, 0, 0,
                                    0, y, 0, y, y, 0, y, 0,
                                    0, 0, y, w, w, y, 0, 0,
                                    y, y, w, 0, 0, w, y, y,
                                    0, 0, w, 0, 0, w, 0, 0,
                                    0, y, 0, w, w, 0, y, 0,
                                    y, 0, y, w, w, y, 0, y,
                                    0, y, 0, 0, 0, 0, y, 0,};
    
    std::uint32_t (*annoyed[2])[8] = {neutral, face2};
    std::uint32_t (*happyface[2])[8] = {neutral, happy};
    while (true) 
    {

        uint32_t current_time = to_ms_since_boot(get_absolute_time());
        if (!gpio_get(Pins::button))
        { 
            frames::button_pressed = true;
        }

        if (frames::button_pressed)
        {
            for (int i = 0; i < 2; ++i)
            {
                face(neutral);
                sleep_ms(200);
                face(face2);
                sleep_ms(200);
            }
            sleep_ms(200);
            frames::button_pressed = false;
        }
        // Check if it's time to switch frames
        if (current_time - frames::last_frame_time >= frames::FRAME_INTERVAL) 
        {
            if (frames::is_face_1) 
            {
                face(happy);
            } 
            else 
            {
                face(neutral);
            }
            frames::is_face_1 = !frames::is_face_1; // Toggle face
            frames::last_frame_time = current_time;
        }
        // small delay for button to be read correctly
        sleep_ms(2);
         
    }
}

