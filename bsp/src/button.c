#include "stm32f4xx.h"
#include "button.h"
#include "systick.h"

extern volatile uint8_t g_mode;

static volatile Button_State_t g_button_state = BUTTON_RELEASED;

void Button_Init(void) {
    /* KÍCH HOẠT CLOCK  */
    RCC->AHB1ENR |= (1UL << 2);   
    RCC->APB2ENR |= (1UL << 14);  
    
    /* CẤU HÌNH GPIO PC13  */
    GPIOC->MODER &= ~(3UL << 26);
    GPIOC->PUPDR &= ~(3UL << 26); 
    GPIOC->PUPDR |= (1UL << 26);  

    /* ÁNH XẠ PC13 -> EXTI13 (SYSCFG)  */
    SYSCFG->EXTICR[3] &= ~(0xFU << 4); 
    SYSCFG->EXTICR[3] |= (0x2U << 4);   

    /*  CẤU HÌNH EXTI LINE 13  */
    EXTI->IMR |= (1UL << 13);     
    EXTI->FTSR |= (1UL << 13);   
    EXTI->RTSR &= ~(1UL << 13);   

    /* CẤU HÌNH NVIC */
    NVIC_SetPriority(EXTI15_10_IRQn, 2); 
    NVIC_EnableIRQ(EXTI15_10_IRQn);      
}

Button_State_t Button_ReadState(void) {
    return g_button_state; 
}

/**
 * @brief Trình xử lý ngắt EXTI15_10 (ISR)
 */
void EXTI15_10_IRQHandler(void) {
    if (EXTI->PR & (1UL << 13)) {
        static uint32_t last_interrupt_time = 0;
        uint32_t current_time = get_tick();

        if (current_time - last_interrupt_time > 50) {
            g_mode = !g_mode; 
            g_button_state = BUTTON_PRESSED;
            
            last_interrupt_time = current_time; 
        }
                EXTI->PR = (1UL << 13);
    }
}
