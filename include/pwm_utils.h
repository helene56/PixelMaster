#ifndef PWM_UTILS 
#define PWM_UTILS
#include <cstdint>
// Interrupt handler for PWM wrap
void on_pwm_wrap();

// Function to start or restart the PWM for the defined cycle count
void start_pwm_for_cycles(uint target_cycles);

void setup_pwm(int pin);
void update_pwm(uint pin, std::int8_t color);
uint get_cycle_count();
void clear();
void set_state(bool toggle);

#endif // PWM_UTILS