#include "core.h"

int main() {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE5) | (1 * GPIO_MODER_MODE5_0);
    while(1) {
        for(volatile int i = 0; i < 1000000; ++i);
        GPIOA->ODR |= (1 << 5);
        for(volatile int i = 0; i < 1000000; ++i);
        GPIOA->ODR &= ~(1 << 5);
    }
    return 0;
}
