# STM32F411RE Bare Metal — Khóa học lập trình MCU từ không

Dự án học tập **lập trình bare metal MCU STM32F411RE** trên Nucleo-F411RE board. Từ GPIO cơ bản đến OTA bootloader, không dùng HAL hay CubeMX.

---

##  Nội dung chính

| Bài | Tên | Nội dung | File chính |
|-----|-----|---------|-----------|
| **L0** | C Foundation | Bit manipulation, ring buffer | `bit_math.h`, `ring_buffer.c` |
| **L1** | Blink LED | GPIO output, ISR vector, linker script | `main.c`, `startup_f4.c` |
| **L2** | SysTick Timer | System tick interrupt, delay | `systick.c`, `systick.h` |
| **L3** | Basic Timer + INT | TIM2 interrupt (1ms) | `tim2.c`, `tim2.h` |
| **L4** | Advanced Timer PWM | PWM output với TIM1 | `tim1_pwm.c`, `tim1_pwm.h` |
| **L5** | UART + NVIC | Serial communication, interrupt | `uart.c`, `uart.h` |
| **L6** | UART + DMA | Transfer data không dùng CPU | `uart_dma.c`, `uart_dma.h` |
| **L7** | Flash Memory | Read/write internal FLASH | `flash.c`, `flash.h` |
| **L8** | OTA Bootloader | Bootloader + firmware update | `boot_f411.ld`, `app_f411.ld` |

---

##  Mục tiêu học tập

1. **Hiểu cấu trúc MCU** — thanh ghi, memory map, boot sequence
2. **Thao tác peripheral** — RCC, GPIO, Timer, UART, DMA
3. **Xử lý interrupt** — ISR vector, NVIC, SysTick
4. **PWM & analog** — PWM output, ADC (optional)
5. **Memory management** — FLASH write/erase, bootloader
6. **Build & debug** — ARM toolchain, OpenOCD, linker script
7. **Comunicación** — UART, serial monitor
8. **OTA firmware update** — bootloader architecture

---

##  Yêu cầu

### Hardware
- **Board**: Nucleo-F411RE (or STM32F411RE)
- **Debugger**: ST-Link V2 (built-in)
- **Cable**: Micro USB (power + debug)
- **Serial adapter** (optional, for UART output): CH340, FT232RL, v.v.

### Software
- **Compiler**: `arm-none-eabi-gcc` (GNU ARM Embedded Toolchain)
- **Build**: `mingw32-make` (Windows) hoặc `make` (Linux/Mac)
- **Programmer**: OpenOCD
- **Terminal**: PuTTY, Tera Term, hoặc `minicom`

### Installation

**Windows (MSYS2)**
```bash
pacman -S mingw-w64-x86_64-arm-none-eabi-gcc
pacman -S mingw-w64-x86_64-arm-none-eabi-gdb
pacman -S mingw-w64-x86_64-openocd
pacman -S make
```

**Linux (Ubuntu)**
```bash
sudo apt install arm-none-eabi-gcc arm-none-eabi-gdb openocd make
```

---

##  Cấu trúc dự án

```
STM32F411RE-Bare-Metal/
├── L0_C_Foundation/          # Nền tảng C (bit macros, ring buffer)
│   ├── bit_math.h
│   ├── ring_buffer.c
│   └── README.md
├── L1_Blink/                 # GPIO LED blink
│   ├── src/
│   │   ├── main.c
│   │   ├── startup_f4.c
│   │   └── bit_math.h
│   ├── linker/
│   │   └── f411re.ld
│   ├── inc/
│   ├── Makefile
│   └── README.md
├── L2_SysTickTimer/          # SysTick interrupt
├── L3_BasicTimer_INT/        # TIM2 interrupt (1ms)
├── L4_AdvancedTimer_PWM/     # TIM1 PWM output
├── L5_Uart_NVIC/             # UART + interrupt
├── L6_Uart_DMA/              # UART + DMA
├── L7_Flash/                 # Internal FLASH read/write
├── L8_OTA/                   # Bootloader + OTA
│   ├── Bootloader/
│   │   ├── boot_f411.ld
│   │   └── Src/main.c
│   ├── Application/
│   │   ├── app_f411.ld
│   │   └── Src/
│   └── send_ota.py           # Python script upload firmware
├── docs/                     # Tài liệu kỹ thuật
│   └── README.md             # Links to PDFs
└── tailieu/                  # Notes, task list
```

---

##  Build & Flash (Tổng quát)

### Build một bài
```bash
cd L1_Blink
mingw32-make              # Build (chỉnh Makefile nếu cần)
mingw32-make size         # Xem kích thước
mingw32-make clean        # Dọn build
```

### Flash
```bash
mingw32-make flash        # Build + flash via OpenOCD
```

**Kết quả:**
```
arm-none-eabi-size build/blink_led.elf
  text    data     bss     dec     hex filename
  1024     256     512    1792     700 build/blink_led.elf

Flashing...
OpenOCD 0.11.0
STM32F4x flash loader, SAM-BA Compliant Bootloader
Firmware programmed successfully!
```

### Serial Monitor
```bash
# Connect to COM port (identify từ Device Manager)
# L5 onwards support UART output
minicom -D /dev/ttyUSB0 -b 9600
# or PuTTY: COM3 @ 9600 baud
```

---

## Trình tự học

**Recommende flow:**
1. **L0** — Hiểu bit manipulation, xem `bit_math.h`, compile `ring_buffer.c`
2. **L1** — Build first LED blink, hiểu boot, linker, ISR vector
3. **L2** — SysTick, non-blocking delay thay dummy loop
4. **L3** — Timer interrupt, chạy task mỗi 1ms
5. **L4** — PWM, điều chỉnh độ sáng LED hoặc servo
6. **L5** — UART + interrupt, gửi data sang computer
7. **L6** — DMA, transfer dữ liệu không dùng CPU
8. **L7** — FLASH, lưu config vào internal memory
9. **L8** — Bootloader, OTA firmware update

---

## Thanh ghi chính (STM32F411RE)

| Peripheral | Base Addr | Clock | Chức năng |
|-----------|-----------|-------|----------|
| **RCC** | 0x40023800 | — | Reset & Clock |
| **GPIOA-E** | 0x40020000-0x40020C00 | AHB1 | General I/O |
| **TIM1** | 0x40010000 | APB2 | Advanced Timer (PWM) |
| **TIM2-5** | 0x40000000-0x40000400 | APB1 | Basic/General Timer |
| **UART1** | 0x40011000 | APB2 | Serial (PA9/PA10) |
| **UART2-4** | 0x40004400-0x40004C00 | APB1 | Serial alternatives |
| **DMA1-2** | 0x40026000-0x40026400 | AHB1 | Direct Memory Access |
| **FLASH** | 0x40023C00 | AHB1 | FLASH control |
| **NVIC** | 0xE000E000 | — | Interrupt controller |

---

## Troubleshooting

**"arm-none-eabi-gcc not found"**
- Cài ARM toolchain từ gnu-mcu-eclipse hoặc MSYS2

**"openocd not found"**
- Cài openocd, hoặc add vào PATH

**Flash lỗi "connection refused"**
- Kiểm tra ST-Link kết nối
- Chạy: `openocd -f interface/stlink.cfg -f target/stm32f4x.cfg`
- Check Device Manager: STM32 STLink

**UART không nhận data**
- Kiểm tra baud rate (thường 9600)
- Xem `uart.h` configure pins (PA9/PA10 for UART1)
- Verify RCC enable cho UART peripheral

**Linker error "undefined reference"**
- Check linker script `*.ld` symbols: `_sidata`, `_sdata`, `_edata`, `_sbss`, `_ebss`
- Xem map file: `cat build/*.map`

---

##  Tài liệu tham khảo

**Tải từ** [st.com](https://www.st.com/en/microcontrollers-microprocessors/stm32f411re.html):
- **RM0383** — Reference Manual (thanh ghi)
- **PM0214** — Cortex-M4 Programming Manual
- **Datasheet** — Pinout, electrical specs
- **SVD file** — debugger support

Xem `docs/README.md` để link tải.

---

