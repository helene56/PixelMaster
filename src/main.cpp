#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include <cstdint>
#include "hardware/pwm.h"
#include <iostream>
#include "led_control.h"
#include "led_memory.h"
#include "animations.h"
#include "pwm_utils.h"

namespace Pins
{
    // Data In pin
    constexpr int DIN {16};
} // namespace Pins


// // Define the target cycle count
// #define TARGET_CYCLE_COUNT 1000  // Run for 1000 cycles

// // Variables to keep track of the PWM slice and cycle count
// volatile uint cycle_count = 0;
// uint slice_num;

// // Interrupt handler for PWM wrap
// void on_pwm_wrap() {
//     // Increment the cycle count each time the PWM wraps
//     cycle_count++;

//     // Check if we've reached the target number of cycles
//     if (cycle_count >= TARGET_CYCLE_COUNT) {
//         // Disable PWM to stop output
//         pwm_set_enabled(slice_num, false);
        
//         // Disable the interrupt as we no longer need it
//         pwm_set_irq_enabled(slice_num, false);
//     }

//     // Clear the interrupt flag to allow future interrupts
//     pwm_clear_irq(slice_num);
// }

// // Function to start or restart the PWM for the defined cycle count
// void start_pwm_for_cycles() {
//     // Reset cycle count to zero
//     cycle_count = 0;

//     // Clear any pending interrupt flag
//     pwm_clear_irq(slice_num);

//     // Enable the PWM wrap interrupt
//     pwm_set_irq_enabled(slice_num, true);

//     // Enable the PWM output
//     pwm_set_enabled(slice_num, true);
// }

int main()
{
    stdio_init_all();
    

    // sleep_ms(20000);
    
    
    // reset_code();
    // send_grb(0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    // reset_code();
    // send_grb(0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    // reset_code();
    // send_grb(0b00000000, 0b00000000, 0b00000000, Pins::DIN);
    // reset_code();
    
    // setLedColor(1, 1,  0b00000000, 0b00001010, 0b00000000, Pins::DIN);
    // reset_code();
    // setLedColor(1, 1,  0b00000000, 0b1010, 0b00000000, Pins::DIN);
    // reset_code();
    
    setup_pwm(Pins::DIN);
    while (true) 
    {
        sleep_ms(2000);  // Wait 2 seconds
        // Restart the PWM for another set of cycles
        // send_grb(0b00000000, 0b00000000, 0b00000000, Pins::DIN);
        update_pwm(Pins::DIN, 156, 50, 10);
        // sleep_ms(10);
        // update_pwm(Pins::DIN, 156, 100, 10);
        // update_pwm(Pins::DIN, 156, 100, 10);
        // gpio_put(Pins::DIN, 0);
        // printf("Hello, world!\n");
        // printf("Current system clock speed: %u Hz\n", clock_get_hz(clk_sys));
        // sleep_ms(1000);
    }
}

