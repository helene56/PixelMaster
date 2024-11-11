#ifndef PIO 
#define PIO

#include <cstdint>

namespace statemachine
{
    // allocate a statemachine
    int sm {pio_claim_unused_sm(pio0, true)};
} // namespace statemachine

void set_gpio(int pin);
void setup_pio();
void load_color(std::uint8_t green, std::uint8_t red, std::uint8_t blue);

#endif // PIO