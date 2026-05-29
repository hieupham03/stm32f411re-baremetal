# L1 — GPIO Blink (PA5)

## Mục Tiêu
Nháy LED PA5 (LED2 Nucleo-F411RE) bằng cách trực tiếp thao tác thanh ghi.

## Không Được Dùng
- HAL / LL / CMSIS
- `stm32f4xx.h` hay bất kỳ header nào từ ST

## Chỉ Được Dùng
- `<stdint.h>` (để có `uint32_t`)
- `#define` với địa chỉ tra tay từ RM0383

---

## Thanh Ghi Cần Đọc (RM0383)

| Thanh ghi | Offset | Chức năng |
|---|---|---|
| `RCC_AHB1ENR` | `RCC+0x30` | Enable clock GPIOA (bit 0) |
| `GPIOA_MODER` | `GPIOA+0x00` | Cấu hình mode PA5 (output) |
| `GPIOA_OTYPER`| `GPIOA+0x04` | Push-pull hay open-drain |
| `GPIOA_BSRR` | `GPIOA+0x18` | Set/Reset PA5 (atomic) |

## Chuỗi Boot Cần Hiểu

```
Power ON
   ↓
CPU đọc [0x08000000] → Initial Stack Pointer (_estack)
   ↓
CPU đọc [0x08000004] → Reset Handler address (_reset)
   ↓
_reset() chạy:
   1. Copy .data: Flash → SRAM
   2. Zero-fill .bss
   3. Gọi main()
   ↓
main() → enable GPIOA clock → config PA5 → toggle loop
```

## Build & Flash

```bash
mingw32-make      # build elf/bin/hex
mingw32-make flash  # flash qua ST-Link
mingw32-make size   # xem kích thước .text .data .bss
mingw32-make clean  # xóa thư mục build
```

> Trên Windows, lệnh `make` thường không có sẵn. Dùng `mingw32-make` nếu bạn cài MinGW/MSYS2.

### Makefile targets

- `all` / `build`: tạo `build/blink_led.elf`, `build/blink_led.bin`, `build/blink_led.hex`
- `flash`: nạp firmware qua OpenOCD
- `size`: in kích thước ELF
- `clean`: xóa thư mục `build`

### Toolchain cần có

- `arm-none-eabi-gcc`
- `arm-none-eabi-objcopy`
- `arm-none-eabi-size`
- `openocd`

### Tập trung học

Makefile ở đây chỉ giữ logic build. Toàn bộ giải thích học tập, chuỗi boot và thanh ghi vẫn nằm trong README để khi đẩy lên GitHub nội dung dễ đọc hơn.

## Bài Tập Mở Rộng

1. **Toggle bằng XOR**: Dùng `GPIOA_ODR ^= (1U << 5)` thay vì BSRR — điều gì nguy hiểm khi dùng cách này trong môi trường có interrupt?
2. **Button PC13**: Đọc trạng thái nút USER (PC13 trên Nucleo), LED sáng khi nhấn.
3. **Đo clock**: Dùng oscilloscope đo tần số toggle, tính lại số cycle của `delay(500000)` ở 16MHz.
4. **Optimize**: Thêm flag `-O2` vào Makefile, build lại. Điều gì xảy ra với loop delay? Fix bằng cách nào?
