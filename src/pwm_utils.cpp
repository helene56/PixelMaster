#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include <cstdint>
#include <stdio.h>

namespace PwmInternal {
    static volatile uint cycle_count {0};  // Internal cycle counter
    static uint slice_num {0};
    static uint target_cycle_count {0};
    static uint pin {0};  // Pin to control, set in setup_pwm
    static bool toggle = false;
    uint static duty_cycle_0 {50};
    uint static duty_cycle_1 {100};
    static std::uint8_t color {0};
    uint static wrap {156};
}
// Interrupt handler for PWM wrap
void on_pwm_wrap() 
{
    PwmInternal::cycle_count++;

    // Alternate between two duty cycles
    if (PwmInternal::toggle) {
        pwm_set_gpio_level(PwmInternal::pin, PwmInternal::duty_cycle_0);
    } else {
        pwm_set_gpio_level(PwmInternal::pin, PwmInternal::duty_cycle_1);
    }

    // Toggle the flag for the next cycle
    PwmInternal::toggle = !PwmInternal::toggle;
    // if (PwmInternal::cycle_count < PwmInternal::target_cycle_count)
    // {
        
    //     bool bit = (PwmInternal::color & (1 << (PwmInternal::target_cycle_count - PwmInternal::cycle_count)));
    //     // Print the bit value
    //     printf("Cycle %u: Bit %u of color is %d\n", 
    //         PwmInternal::cycle_count, 
    //         7 - PwmInternal::cycle_count, 
    //         bit);
    //     // Set the duty cycle based on the current bit
    //     if (bit) 
    //     {
    //         pwm_set_gpio_level(PwmInternal::pin, PwmInternal::duty_cycle_1);
    //     } 
    //     else 
    //     {
    //         pwm_set_gpio_level(PwmInternal::pin, PwmInternal::duty_cycle_0);
    //     }
    //     // PwmInternal::cycle_count++;
        
    // }
    

    // Check if we've reached the target number of cycles
    if (PwmInternal::cycle_count >= PwmInternal::target_cycle_count) {

        pwm_set_wrap(PwmInternal::slice_num, 0);  // Set wrap to zero
        pwm_set_gpio_level(PwmInternal::pin, 0);  // Ensure the output level is low
        
        // Disable the interrupt as we no longer need it
        pwm_set_irq_enabled(PwmInternal::slice_num, false);
        // Disable PWM to stop output
        pwm_set_enabled(PwmInternal::slice_num, false);

        }

    // Clear the interrupt flag to allow future interrupts
    pwm_clear_irq(PwmInternal::slice_num);
}

// Function to start or restart the PWM for the defined cycle count
void start_pwm_for_cycles(uint target_cycles) 
{
    // // Reset cycle count to zero
    // PwmInternal::cycle_count = 0;
    PwmInternal::target_cycle_count = target_cycles;
    // Clear any pending interrupt flag
    pwm_clear_irq(PwmInternal::slice_num);

    // Enable the PWM wrap interrupt
    pwm_set_irq_enabled(PwmInternal::slice_num, true);

    // Enable the PWM output
    pwm_set_enabled(PwmInternal::slice_num, true);
}


void setup_pwm(int pin)
{
    // Set up the GPIO pin for PWM output
    gpio_set_function(pin, GPIO_FUNC_PWM);
    PwmInternal::slice_num = pwm_gpio_to_slice_num(pin);
    PwmInternal::pin = pin;
    // Get the PWM slice number for the selected pin
    // PwmInternal::slice_num = pwm_gpio_to_slice_num(pin);
    // Calculate the PWM frequency for a 1,25 µs period
    // The system clock runs at 125 MHz by default
    // 1,25 chosen because: a high of 0.4 us and a low of 0.85 us
    // 1,25 µs period => 1 / 1,25 µs = 0.8 kHz (approx.)
    // PWM count = frequency * period time
    // Wrap value for 1,25 µs period with 125 MHz clock = 125 MHz * 1,25us = 156
    // wrap = 156 - 1
    // pwm_set_wrap(PwmInternal::slice_num, wrap - 1); // Set wrap value for 12 µs period
    
    // // Set duty cycle to achieve 0.4 µs high and 0.85 µs low
    // // Duty cycle for high time = (high time / period time) * wrap value
    // // High time = 0.4 µs, period = 1,25 µs
    // // Duty cycle = (0.4 µs / 1,25 µs) * 156 = 50
    // pwm_set_gpio_level(pin, duty_cycle); // Set duty cycle for 4 µs high

    // Set the IRQ handler and enable it
    irq_set_exclusive_handler(PWM_IRQ_WRAP, on_pwm_wrap);
    irq_set_enabled(PWM_IRQ_WRAP, true);

    // // Start the PWM for the defined number of cycles
    // start_pwm_for_cycles(target_cycles);
}

void update_pwm(uint pin, std::int8_t color) 
{
    // Reset cycle count to zero
    PwmInternal::cycle_count = 0;
    PwmInternal::color = color;

    uint initial_duty_cycle = (PwmInternal::color & (1 << 7)) ? PwmInternal::duty_cycle_1 : PwmInternal::duty_cycle_0;
    // uint initial_duty_cycle = PwmInternal::duty_cycle_0;
    pwm_set_wrap(PwmInternal::slice_num, PwmInternal::wrap - 1);      // Update PWM period
    pwm_set_gpio_level(pin, initial_duty_cycle);    // Update PWM duty cycle
    // Start the PWM for the defined number of cycles
    start_pwm_for_cycles(8);
}


void clear()
{
    // Disable PWM temporarily to apply new settings cleanly
    pwm_set_enabled(PwmInternal::slice_num, false);
}
// Getter function to expose cycle count safely
uint get_cycle_count()
{
    return PwmInternal::cycle_count;
}

void set_state(bool toggle)
{
    PwmInternal::toggle = toggle;
}