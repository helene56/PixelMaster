#include <stdio.h>
#include "pico/stdlib.h"

constexpr int PIN16 {16};

int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}

void T0H()
{
    
}
