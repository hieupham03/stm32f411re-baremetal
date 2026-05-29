/*
 * NHIỆM VỤ: Viết main.c cho bài L1 — Nháy LED PA5
 * =================================================
 *
 * KIẾN THỨC CẦN HỌC TRƯỚC:
 *   - RM0383 Chương 6: Reset and clock control (RCC)
 *     Tập trung vào: RCC_AHB1ENR — enable clock cho GPIO
 *   - RM0383 Chương 8: General-purpose I/Os (GPIO)
 *     Tập trung vào: MODER, OTYPER, OSPEEDR, PUPDR, ODR, BSRR
 *   - Hiểu pattern: (*(volatile uint32_t *)(address))
 *     volatile là gì? Tại sao cần khi đọc/ghi thanh ghi phần cứng?
 *
 * CÂU HỎI CẦN TRẢ LỜI TRƯỚC KHI CODE:
 *   1. Địa chỉ base của RCC là bao nhiêu? (tra RM0383 §6.3)
 *   2. Offset của RCC_AHB1ENR là bao nhiêu? Bit nào enable GPIOA?
 *   3. Địa chỉ base của GPIOA là bao nhiêu?
 *   4. MODER register: mỗi pin chiếm bao nhiêu bit? PA5 ở bit nào?
 *      Giá trị nào = Input? Output? Alternate Function? Analog?
 *   5. BSRR khác ODR thế nào? Tại sao BSRR an toàn hơn khi có interrupt?
 *   6. LED2 trên Nucleo-F411RE kết nối với chân nào? Mức nào thì LED sáng?
 *
 * NHIỆM VỤ PHẢI LÀM (theo thứ tự):
 *
 *   [1] Dùng #define để map địa chỉ thanh ghi cần dùng:
 *       RCC_AHB1ENR, GPIOA_MODER, GPIOA_OTYPER, GPIOA_BSRR
 *       (Tự tra địa chỉ từ RM0383, không copy từ đâu)
 *
 *   [2] Viết hàm delay(count) — busy-wait loop:
 *       - Tại sao tham số phải là volatile?
 *       - Hàm này KHÔNG chính xác — đó là bình thường ở L1
 *
 *   [3] Viết hàm main():
 *       - Enable clock GPIOA (RCC_AHB1ENR)
 *       - Cấu hình PA5 thành Output push-pull
 *         (Clear bits trước, rồi mới Set — tại sao phải làm vậy?)
 *       - Loop vô hạn: Toggle PA5 với delay ≈ 500ms
 *         Dùng BSRR để set và reset, không dùng ODR trực tiếp
 */


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
        delay_dummy(1000000); 
    }
    
    return 0;
}