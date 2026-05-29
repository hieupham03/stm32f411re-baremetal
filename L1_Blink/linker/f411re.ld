/*
 * NHIỆM VỤ: Viết linker script cho STM32F411RE
 * =============================================
 *
 * KIẾN THỨC CẦN HỌC TRƯỚC:
 *   - RM0383 Chương 3: Memory and bus architecture (memory map)
 *   - PM0214 Mục 2.2: Memory model Cortex-M4
 *   - GNU LD manual: MEMORY, SECTIONS, AT, LOADADDR, KEEP, ENTRY
 *     https://sourceware.org/binutils/docs/ld/
 *
 * CÂU HỎI CẦN TRẢ LỜI TRƯỚC KHI CODE:
 *   1. Flash của F411RE bắt đầu từ địa chỉ nào? Kích thước bao nhiêu?
 *   2. SRAM của F411RE bắt đầu từ địa chỉ nào? Kích thước bao nhiêu?
 *   3. VMA (Virtual Memory Address) và LMA (Load Memory Address) khác nhau thế nào?
 *   4. Tại sao .vectors phải là section đầu tiên trong Flash?
 *   5. Symbol _estack tính như thế nào từ SRAM?
 *   6. KEEP() dùng để làm gì?
 *
 * NHIỆM VỤ PHẢI LÀM (theo thứ tự):
 *
 *   [1] Khai báo stack size (ví dụ 4KB = 0x1000)
 *
 *   [2] Viết block MEMORY:
 *       - Vùng flash: origin, length — tra RM0383 §3.3 (memory map)
 *       - Vùng sram:  origin, length — tra RM0383 §3.3
 *       - Thuộc tính: (rx) cho flash, (rwx) cho sram
 *
 *   [3] Khai báo ENTRY(_reset)
 *
 *   [4] Viết block SECTIONS:
 *       - .vectors  → đặt vào flash, KEEP để linker không bỏ đi
 *       - .text     → code + rodata → flash
 *       - .data     → VMA = sram, LMA = flash (dùng "AT > flash")
 *                     Định nghĩa symbols: _sdata, _edata, _sidata
 *       - .bss      → sram, không chiếm chỗ trong Flash
 *                     Định nghĩa symbols: _sbss, _ebss
 *       - Tính _estack = đỉnh SRAM (ORIGIN(sram) + LENGTH(sram))
 */
