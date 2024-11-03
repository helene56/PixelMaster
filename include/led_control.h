#ifndef LED_CONTROL 
#define LED_CONTROL

#include <cstdint>

// loop to waste clock cycles, to have precise control over the wait time
inline void busy_wait_cycles(uint32_t cycles);
inline void wait_0_4us();
inline void wait_0_45us();
inline void wait_0_8us();
inline void wait_0_85us();

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
