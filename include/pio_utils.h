#ifndef PIO_UTILS 
#define PIO_UTILS

#include "hardware/pio.h"

inline void put_pixel(uint32_t pixel_grb) 
{  
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8);
}

inline uint32_t ugrb_u32(uint8_t g, uint8_t r, uint8_t b)
{
    return ((uint32_t) (g) << 16) | ((uint32_t) (r) << 8) |(uint32_t) (b);
}

inline void reset_pixel()
{
    sleep_us(100);
}

#endif // PIO_UTILS