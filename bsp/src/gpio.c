#include "stm32f4xx.h"
#include "gpio.h"

void GPIO_InitLED(void) {

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3UL << 10);
    GPIOA->MODER |= (1UL << 10);

    GPIOA->OSPEEDR &= ~(3UL << 10);

    GPIOA->OTYPER &= ~(1UL << 5);

    GPIOA->PUPDR &= ~(3UL << 10);
}

void GPIO_WriteLED(uint8_t state) {
    if (state != 0) {
        GPIOA->BSRR = (1UL << 5);
    }
    else {
        GPIOA->BSRR = (1UL << 21);
    }
}

void GPIO_ToggleLED(void) {
    GPIOA->ODR ^= (1UL << 5);
}
