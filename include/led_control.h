#ifndef LED_CONTROL 
#define LED_CONTROL

#include <cstdint>

// loop to waste clock cycles, to have precise control over the wait time
// inline void busy_wait_cycles(uint32_t cycles);
// inline void wait_0_4us();
// inline void wait_0_45us();
// inline void wait_0_8us();
// inline void wait_0_85us();

// clock running up to 133 MHz -- 1/ 133MHz = 7.52 nanoseconds

inline void busy_wait_cycles(uint32_t cycles) 
{
    // A simple loop to waste clock cycles
    // Adjusted loop to compensate for overhead
    uint32_t adjusted_cycles = cycles / 11;  // Each iteration takes ~11 cycles
    for (volatile uint32_t i = 0; i < adjusted_cycles; ++i) {
        // No operation, just waste cycles
        __asm volatile ("nop");
    }
}

// Busy-wait for approximately 0.4 µs (400 ns)
inline void wait_0_4us() 
{
    // 0.4 µs requires about 53 clock cycles at 133 MHz
    busy_wait_cycles(50);
}
// running at 125 MHz, 8 ns/clock
// Busy-wait for approximately 0.45 µs (450 ns)
// 450 ns / 7.52 ns/clock
inline void wait_0_45us() 
{
    // 0.45 µs requires about 59 clock cycles at 133 MHz
    busy_wait_cycles(56);
}

// Busy-wait for approximately 0.8 µs (800 ns)
inline void wait_0_8us() 
{
    // 0.8 µs requires about 106 clock cycles at 133 MHz
    busy_wait_cycles(100);
}

// Busy-wait for approximately 0.85 µs (850 ns)
inline void wait_0_85us() 
{
    // 0.85 µs requires about 113 clock cycles at 133 MHz
    // turning it a bit down, it is more slow
    busy_wait_cycles(106);
}

// helper function to send 8 color bits
void send_color_bits(std::uint8_t color, int pin);
// send a 24 bits grb code to a led
void send_grb(std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin);
// Reset after sending all the color data
void reset_code();
// send bit 0
void send_bit_0(int pin);
// send bit 1
void send_bit_1(int pin);
// set color at specfic row, col position
void setLedColor(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin);

void storeLed(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue, int pin);

void sendLed(int pin);

#endif // LED_CONTROL
