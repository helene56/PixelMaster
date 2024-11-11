#include "hardware/pio.h"
#include "pico/stdlib.h"

namespace statemachine
{
    int sm {pio_claim_unused_sm(pio0, true)};
} // namespace statemachine

// allocate a statemachine


void set_gpio(int pin)
{
    pio_gpio_init(pio0, pin);
    pio_sm_set_consecutive_pindirs(pio0, statemachine::sm, pin, 1, true);
}

void setup_pio()
{
    pio_sm_set_enabled(pio0, statemachine::sm, true);
}
