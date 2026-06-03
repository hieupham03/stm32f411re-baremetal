#include "stm32f4xx.h"
#include "systick.h"


static volatile uint32_t ms_ticks = 0;

void SysTick_Init(uint32_t system_clock_hz) {
    SysTick->CTRL = 0;
    
    SysTick->LOAD = (system_clock_hz / 1000U) - 1U;
    
    SysTick->VAL = 0;
    
    NVIC_SetPriority(SysTick_IRQn, 15);

    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
}

uint32_t get_tick(void) {
    return ms_ticks;
}

void delay_ms(uint32_t ms) {
    uint32_t start = get_tick();
    while ((get_tick() - start) < ms) {
        
    }
}

void SysTick_Handler(void) {
    ms_ticks++;
}
