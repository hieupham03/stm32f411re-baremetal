# L1_Blink

Bài L1 nháy LED PA5 (LED2 Nucleo-F411RE) bằng cách trực tiếp thao tác thanh ghi. Đây là bài đầu tiên để hiểu cách khởi động MCU, cấu hình GPIO và build bare metal firmware.

## Nội dung chính

- `src/main.c` — toggle LED PA5
- `src/startup_f4.c` — reset handler & ISR vector
- `linker/f411re.ld` — linker script

## Mục tiêu học tập

1. Hiểu cách **khởi động MCU** từ zero (boot sequence, ISR vector)
2. Nắm vững **thao tác thanh ghi RCC** để enable clock
3. Cấu hình **GPIO mode** cho output
4. Toggle **GPIO output** bằng ODR register
5. Sử dụng **toolchain ARM** để build & flash


## Thanh ghi cần biết (RM0383)

| Thanh ghi | Địa chỉ | Chức năng |
|-----------|--------|----------|
| `RCC_AHB1ENR` | 0x40023830 | Enable clock GPIOA (bit 0) |
| `GPIOA_MODER` | 0x40020000 | Mode PA5 (bits [11:10] = 01 for output) |
| `GPIOA_ODR` | 0x40020014 | Output data PA5 (bit 5) |



## Build & Flash

### Build
```bash
cd L1_Blink
mingw32-make
```

Output: `build/blink_led.elf`, `build/blink_led.hex`, `build/blink_led.bin`

### Flash
```bash
mingw32-make flash
```

Kết quả: LED green (PA5) nháy liên tục.

### Kiểm tra
```bash
mingw32-make size    # Xem kích thước
mingw32-make clean   # Dọn build
```

---

## Giải thích code

### main.c
```c
#include "bit_math.h"

#define RCC_AHB1ENR  (*(volatile uint32_t *)0x40023830)
#define GPIOA_MODER  (*(volatile uint32_t *)0x40020000)
#define GPIOA_ODR    (*(volatile uint32_t *)0x40020014)

int main(void) {
    SET_BIT(RCC_AHB1ENR, 0);    // Enable GPIOA clock
    CLEAR_BIT(GPIOA_MODER, 11); // PA5 mode = 01 (output)
    SET_BIT(GPIOA_MODER, 10);
    
    while(1) {
        TOGGLE_BIT(GPIOA_ODR, 5);  // Toggle PA5
        delay_dummy(1000000);
    }
}
```

### startup_f4.c
```c
extern uint32_t _estack, _sidata, _sdata, _edata, _sbss, _ebss;

void Reset_Handler(void) {
    // Copy .data from FLASH to SRAM
    uint32_t *src = &_sidata, *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;
    
    // Zero .bss
    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;
    
    main();
}

__attribute__((section(".isr_vector")))
uint32_t* isr_vector[] = {
    (uint32_t*)&_estack,
    (uint32_t*)Reset_Handler,
    (uint32_t*)Default_Handler,
};
```

### f411re.ld (Linker script)
```ld
MEMORY {
    FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 512K
    SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS {
    .isr_vector : { KEEP(*(.isr_vector)) } > FLASH
    
    .data : {
        _sidata = LOADADDR(.data);
        _sdata = .;
        *(.data)
        _edata = .;
    } > SRAM AT> FLASH
    
    .bss : {
        _sbss = .;
        *(.bss)
        _ebss = .;
    } > SRAM
}
```

---



