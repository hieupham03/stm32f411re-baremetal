# L0_C_Foundation

Bài L0 là nền tảng C cơ bản cho toàn bộ dự án STM32F411RE bare-metal. Mục tiêu của bài này là làm quen với thao tác bit, macro và cấu trúc dữ liệu đơn giản trước khi chuyển sang lập trình vi điều khiển.

## Nội dung chính

- `bit_math.h` — các macro thao tác bit cơ bản
- `ring_buffer.c` — ví dụ cấu trúc vòng (ring buffer)

## Mục tiêu học tập

1. Hiểu cách dùng toán tử bit trong C để:
   - set/clear/toggle bit
   - đọc giá trị bit
2. Làm quen với các macro dùng cho thao tác thanh ghi hoặc biến bit
3. Hiểu khái niệm buffer vòng (ring buffer) và cách cài đặt đơn giản


Macro hữu ích khi thao tác trực tiếp với thanh ghi trên MCU:

- `SET_BIT(REG, BIT)` — đặt bit về 1
- `CLEAR_BIT(REG, BIT)` — đặt bit về 0
- `TOGGLE_BIT(REG, BIT)` — đảo trạng thái bit
- `READ_BIT(REG, BIT)` — đọc trạng thái bit

Các macro này dùng nhiều trong lập trình bare-metal để thao tác GPIO, RCC, timer, UART,...

### `ring_buffer.c`

Đây là ví dụ xử lý buffer vòng. Buffer vòng thường dùng trong:

- giao tiếp UART
- DMA nhận dữ liệu
- xử lý dữ liệu streaming

Nó giúp lưu dữ liệu theo chế độ FIFO mà không cần dịch toàn bộ mảng.


