#ifndef PWM_UTILS 
#define PWM_UTILS

// Interrupt handler for PWM wrap
void on_pwm_wrap();

// Function to start or restart the PWM for the defined cycle count
void start_pwm_for_cycles(uint target_cycles);

void setup_pwm(int pin);
void update_pwm(uint pin, uint new_wrap, uint new_duty_cycle, uint target_cycles);

uint get_cycle_count();
void clear();

#endif // PWM_UTILS