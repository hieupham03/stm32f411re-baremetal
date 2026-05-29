/*
 * NHIỆM VỤ: Viết startup file cho STM32F411RE
 * =============================================
 *
 * File này chạy TRƯỚC main(). Đây là code đầu tiên CPU thực thi sau reset.
 *
 * KIẾN THỨC CẦN HỌC TRƯỚC:
 *   - RM0383 Chương 10: Interrupt and exception management
 *   - PM0214 Mục 2.3: Vector table (Cortex-M4 exception model)
 *   - PM0214 Mục 2.1: Programmer's model (thanh ghi SP, PC)
 *
 * CÂU HỎI CẦN TRẢ LỜI TRƯỚC KHI CODE:
 *   1. Vector table là gì? Nằm ở đâu trong Flash?
 *   2. Tại sao entry đầu tiên của vector table là Stack Pointer, không phải code?
 *   3. CPU nhảy vào địa chỉ nào sau khi reset? Lấy giá trị đó từ đâu?
 *   4. .data section là gì? Tại sao phải copy từ Flash sang SRAM?
 *   5. .bss section là gì? Tại sao phải zero-fill?
 *   6. weak attribute và alias hoạt động như thế nào trong GCC?
 *
 * NHIỆM VỤ PHẢI LÀM (theo thứ tự):
 *
 *   [1] Khai báo extern các symbols từ linker script:
 *       _estack, _sdata, _edata, _sidata, _sbss, _ebss
 *
 *   [2] Viết hàm Default_Handler:
 *       - Loop vô hạn
 *       - Dùng làm handler mặc định cho mọi IRQ chưa implement
 *
 *   [3] Khai báo tất cả exception/IRQ handler với weak alias → Default_Handler:
 *       - System exceptions: NMI, HardFault, MemManage, BusFault,
 *         UsageFault, SVC, PendSV, SysTick
 *       - External IRQs cần cho 8 bài học: TIM2, USART2, DMA1_Stream5
 *       - Tra RM0383 Table 38 để biết đủ 86 external IRQ của F411
 *
 *   [4] Định nghĩa vector table:
 *       - Đặt vào section ".vectors" bằng __attribute__((section(".vectors")))
 *       - Entry 0: địa chỉ của _estack (ép kiểu thành function pointer)
 *       - Entry 1: _reset handler
 *       - Entry 2+: các exception/IRQ handler theo đúng thứ tự RM0383 Table 38
 *       - STM32F411 có 16 system exceptions + 86 external IRQ = 102 entries
 *
 *   [5] Viết hàm _reset (Reset Handler):
 *       - Bước 1: Copy .data từ Flash (địa chỉ LMA = _sidata) sang SRAM (từ _sdata đến _edata)
 *       - Bước 2: Zero-fill .bss từ _sbss đến _ebss
 *       - Bước 3: Gọi main()
 *       - Bước 4: Loop vô hạn (phòng trường hợp main() return)
 */
