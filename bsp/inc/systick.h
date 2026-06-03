#ifndef BSP_SYSTICK_H
#define BSP_SYSTICK_H

#include <stdint.h>

void SysTick_Init(uint32_t system_clock_hz);
uint32_t get_tick(void);
void delay_ms(uint32_t ms);

#endif // BSP_SYSTICK_H
