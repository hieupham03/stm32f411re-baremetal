#include "stm32f4xx.h"

/*
 * NHIỆM VỤ: Hoàn thành cấu hình Clock hệ thống lên 100MHz dùng PLL & HSI
 * ======================================================================
 * Mặc định sau Reset, MCU chạy bằng dao động nội HSI (16MHz).
 * Để nâng hiệu năng lên tối đa 100MHz, ta cần dùng bộ nhân tần PLL.
 *
 * KIẾN THỨC CẦN HỌC TRƯỚC:
 * - RM0383 §6: Reset and clock control (RCC)
 * - RM0383 §3.4: FLASH memory read interface (Cấu hình Wait States tương ứng với tần số HCLK)
 *
 * CÂU HỎI CẦN TRẢ LỜI TRƯỚC KHI CODE:
 * 1. Tại sao khi nâng tần số hoạt động của MCU ta lại phải cấu hình FLASH Latency (Wait States)?
 *    (Gợi ý: Tốc độ truy xuất của bộ nhớ Flash chậm hơn tốc độ của nhân CPU khi chạy tần số cao).
 * 2. Tần số hoạt động tối đa của Bus APB1 và APB2 trên STM32F411 là bao nhiêu?
 *    (Gợi ý: Tra bảng APB clock frequency limits trong RM).
 */

void SystemInit(void) {
    /* 1) Enable power controller clock and set voltage scaling to Scale 1 */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
    PWR->CR = (PWR->CR & ~PWR_CR_VOS) | PWR_CR_VOS;

    /* 2) Enable internal HSI oscillator and wait until ready */
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0U) {
        /* wait */
    }

    /* 3) Configure Flash latency, prefetch and caches for 100 MHz */
    FLASH->ACR |= FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_3WS;

    /* 4) Configure bus prescalers: AHB = DIV1, APB1 = DIV2, APB2 = DIV1 */
    RCC->CFGR &= ~(RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2);
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1;

    /* 5) Configure PLL: HSI -> PLL -> 100 MHz SYSCLK */
    RCC->PLLCFGR = (16U << RCC_PLLCFGR_PLLM_Pos)
                 | (400U << RCC_PLLCFGR_PLLN_Pos)
                 | RCC_PLLCFGR_PLLP_0
                 | RCC_PLLCFGR_PLLSRC_HSI
                 | (8U << RCC_PLLCFGR_PLLQ_Pos);

    /* 6) Enable PLL and wait for PLL ready */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) {
        /* wait */
    }

    /* 7) Select PLL as system clock source and wait until switched */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL) {
        /* wait */
    }
}
