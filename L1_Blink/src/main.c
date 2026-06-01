  
#include <stdint.h>
#include "bit_math.h" 

#define RCC_BASE      0x40023800
#define RCC_AHB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x30))

#define GPIOA_BASE    0x40020000
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x14))


void delay_dummy(volatile uint32_t count) {
    while(count--) {
    }
}

int main(void) {

    SET_BIT(RCC_AHB1ENR, 0);
    CLEAR_BIT(GPIOA_MODER, 11); 
    SET_BIT(GPIOA_MODER, 10);
    while(1) {
        TOGGLE_BIT(GPIOA_ODR, 5);
        delay_dummy(5000000); 
    }
    
    return 0;
}