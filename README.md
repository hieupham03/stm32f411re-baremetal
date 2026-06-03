# Dự án Lập trình Bare-Metal STM32F411RE

Dự án này tập trung vào việc nghiên cứu và phát triển mã nguồn ở cấp độ thanh ghi (register level) cho vi điều khiển STM32F411RE trên board mạch Nucleo-F411RE. Mã nguồn được phát triển trực tiếp thông qua các file định nghĩa thanh ghi chuẩn CMSIS, không sử dụng các thư viện trừu tượng như STM32Cube HAL hay LL.

---

## Kiến trúc phần mềm

Hệ thống được tổ chức theo cấu trúc phân lớp (Layered Architecture):

* **app/**: Tầng ứng dụng (Application Layer). Chứa logic nghiệp vụ chính của hệ thống và độc lập với phần cứng.
  * `app/src/main.c`: Điểm khởi đầu của chương trình và vòng lặp giám sát tác vụ.
* **bsp/** (Board Support Package): Tầng hỗ trợ phần cứng. Chứa các driver điều khiển ngoại vi ở mức thanh ghi do lập trình viên tự xây dựng.
  * `bsp/src/gpio.c`: Driver cấu hình và điều khiển các chân GPIO.
  * `bsp/src/systick.c`: Driver cấu hình ngắt thời gian thực SysTick để đo lường thời gian.
* **core/**: Tầng khởi động và cấu hình cốt lõi của vi điều khiển.
  * `core/src/startup_stm32f411xe.c`: File khởi động viết bằng ngôn ngữ C (thực hiện cấu hình phân vùng RAM/BSS và thiết lập Vector Table).
  * `core/src/system_stm32f4xx.c`: Hàm khởi tạo hệ thống và cấu hình clock PLL lên tần số 100MHz từ nguồn HSI.
  * `core/inc/`: Chứa các file tiêu chuẩn CMSIS định nghĩa địa chỉ thanh ghi của chip.
* **Makefile**: Kịch bản quản lý quá trình biên dịch và liên kết mã nguồn.
* **f411re.ld**: Linker script chịu trách nhiệm phân bổ sơ đồ bộ nhớ FLASH và SRAM vật lý.

---

## Yêu cầu hệ thống

### Phần cứng
* Board mạch Nucleo-F411RE (hoặc mạch phát triển sử dụng chip STM32F411RE).
* Cáp USB kết nối để nạp chương trình và giao tiếp cổng nối tiếp (UART).

### Công cụ phát triển
* Trình biên dịch: `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain).
* Công cụ build: `make` (đối với Linux/Mac) hoặc `mingw32-make` (đối với Windows).
* Công cụ nạp và gỡ lỗi: `openocd`.

---

## Hướng dẫn vận hành

Thực hiện các lệnh sau tại thư mục gốc của dự án thông qua Terminal:

### Làm sạch thư mục build cũ
```bash
mingw32-make clean
```

### Biên dịch dự án
```bash
mingw32-make
```

### Nạp chương trình vào vi điều khiển
```bash
mingw32-make flash
```

---

## Lộ trình phát triển Driver hệ thống

Quy trình tự xây dựng các module driver từ mức cơ bản đến nâng cao:

1. **Khởi động hệ thống và GPIO**: Hiện thực hóa cơ chế khởi tạo bộ nhớ RAM/BSS trong file Startup và điều khiển xuất nhập GPIO cơ bản (nháy LED).
2. **Cấu hình PLL Clock và SysTick**: Thiết lập clock hệ thống đạt 100MHz bằng bộ nhân tần PLL và viết driver định thời ngắt SysTick (non-blocking delay).
3. **Ngắt Timer cơ bản**: Cấu hình bộ định thời Timer 2 tạo ngắt chu kỳ 1ms phục vụ lập lịch.
4. **Bộ điều chế độ rộng xung (PWM)**: Cấu hình Timer phát xung PWM ứng dụng điều khiển thiết bị ngoại vi.
5. **Giao tiếp cổng nối tiếp (UART)**: Truyền nhận dữ liệu với máy tính thông qua cơ chế ngắt NVIC.
6. **Bộ truy cập bộ nhớ trực tiếp (DMA)**: Cấu hình UART kết hợp với DMA để truyền nhận dữ liệu tốc độ cao, giải phóng tài nguyên CPU.
7. **Bộ nhớ Flash nội**: Thực hiện các thao tác đọc/ghi dữ liệu cấu hình vào bộ nhớ Flash tích hợp của chip.
8. **OTA Bootloader**: Thiết kế phân vùng nhớ và lập trình chương trình Bootloader để cập nhật chương trình từ xa thông qua giao thức tự chọn.
