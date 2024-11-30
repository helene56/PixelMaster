#ifndef LED_CONTROL 
#define LED_CONTROL

#include <cstdint>

// set color at specfic row, col position
void setLedColor(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue);

void storeLed(int row, int col, std::uint8_t green, std::uint8_t red, std::uint8_t blue);

void sendLed();

void resetLedmemory();

#endif // LED_CONTROL
