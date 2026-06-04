# HƯỚNG DẪN CẤU HÌNH GDB & PHÂN TÍCH LỖI HARD FAULT

Tài liệu này cung cấp hướng dẫn từng bước để thiết lập môi trường debug bằng GDB, ST-Link, OpenOCD trên VS Code và cách phân tích nguyên nhân gây ra lỗi Hard Fault trên vi điều khiển STM32F411RE.

---

## PHẦN 1: CẤU HÌNH CÔNG CỤ DEBUG TRÊN VS CODE

Để debug trực quan trên VS Code, ta cần sử dụng extension **Cortex-Debug** kết hợp với **OpenOCD** và **GNU Arm Embedded Toolchain**.

### 1. Các phần mềm cần cài đặt:
1. Extension VS Code: `Cortex-Debug` (bởi marus25).
2. Đường dẫn của `arm-none-eabi-gdb` và `openocd` phải được thêm vào biến môi trường hệ thống (System PATH).

### 2. Cấu hình file `.vscode/launch.json`:
Đảm bảo file `.vscode/launch.json` ở thư mục gốc được cấu hình để trỏ đúng file `.elf` được tạo từ Makefile ở thư mục root. Cấu hình mẫu chuẩn:

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Bare-Metal (OpenOCD)",
            "type": "cortex-debug",
            "request": "launch",
            "servertype": "openocd",
            "executable": "${workspaceFolder}/build/app.elf",
            "configFiles": [
                "interface/stlink.cfg",
                "target/stm32f4x.cfg"
            ],
            "runToEntryPoint": "main",
            "preLaunchTask": "Build Project"
        }
    ]
}
```

### 3. Cấu hình file `.vscode/tasks.json`:
File này định nghĩa task tự động biên dịch trước khi bắt đầu debug (`preLaunchTask`):

```json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Build Project",
            "type": "shell",
            "command": "mingw32-make",
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "problemMatcher": ["$gcc"]
        }
    ]
}
```

---

## PHẦN 2: THỰC HÀNH GỠ LỖI HARD FAULT

Lỗi **Hard Fault** xảy ra khi bộ vi xử lý thực hiện một lệnh không hợp lệ, cố gắng truy cập địa chỉ nhớ không tồn tại, ghi vào vùng nhớ Read-Only, hoặc do lỗi bus, lỗi chia cho 0.

### 1. Cách mô phỏng lỗi Hard Fault
Trong file `app/src/main.c`, ta thực hiện hành vi viết dữ liệu vào con trỏ NULL:
```c
volatile uint32_t *ptr = (volatile uint32_t *)0x00000000;
*ptr = 0xDEADBEEF; // Địa chỉ 0x0 thuộc Flash (Read-Only) -> Báo lỗi ghi bất hợp pháp lập tức!
```

### 2. Quy trình gỡ lỗi từng bước bằng Debugger (VS Code + GDB):

1. **Đặt Breakpoint**: Mở file `core/src/startup_stm32f411xe.c`, tìm hàm `HardFault_Handler(void)` và đặt một breakpoint (nhấn F9) tại dòng vòng lặp vô hạn `while(1);` bên trong hàm này.
2. **Bắt đầu Debug**: Nhấn F5 (hoặc vào tab Run & Debug và chọn "Debug Bare-Metal (OpenOCD)"). Chương trình sẽ dừng ở đầu hàm `main()`.
3. **Chạy đến điểm Crash**: Nhấn Continue (F5), chương trình sẽ chạy, gây ra lỗi truy cập địa chỉ 0x0 và lập tức dừng lại tại Breakpoint ở `HardFault_Handler`.
4. **Kiểm tra các thanh ghi lõi (Core Registers)**:
   - Trong panel **Variables** (hoặc tab **Registers** của Cortex-Debug), tìm danh sách các thanh ghi CPU.
   - Quan sát giá trị của 2 thanh ghi quan trọng nhất:
     - **PC (Program Counter)**: Chỉ địa chỉ lệnh tiếp theo CPU chuẩn bị thực thi. Tuy nhiên, khi rơi vào ngắt, PC sẽ chỉ địa chỉ trong `HardFault_Handler`. Do đó ta cần tìm địa chỉ lúc lỗi xảy ra.
     - **LR (Link Register)**: Chứa địa chỉ trả về (Return Address) khi thực hiện lệnh gọi hàm. Khi Hard Fault xảy ra, CPU sẽ tự động đẩy một cấu trúc gọi là **Stack Frame** vào Stack (bao gồm các thanh ghi R0-R3, R12, LR, PC, xPSR tại thời điểm xảy ra lỗi).

### 3. Tìm dòng code gây lỗi bằng Command GDB (Nâng cao):

Khi dừng ở `HardFault_Handler`, ta có thể tìm ra chính xác PC và LR tại thời điểm xảy ra crash bằng cách đọc vùng nhớ Stack. 
Vì CPU Cortex-M4 đẩy Stack Frame lên MSP (Main Stack Pointer) hoặc PSP (Process Stack Pointer) khi nhảy vào ngắt:

Nhập lệnh sau vào ô **Debug Console** của VS Code để hiển thị các thanh ghi trong Stack Frame:
```text
-exec x/8xw $sp
```
*(Lệnh này in ra 8 word dữ liệu bắt đầu từ đỉnh Stack `$sp`)*

Cấu trúc Stack Frame được đẩy tự động bởi Cortex-M4:
1. `sp[0]` = R0
2. `sp[1]` = R1
3. `sp[2]` = R2
4. `sp[3]` = R3
5. `sp[4]` = R12
6. `sp[5]` = **LR** (Link Register trước khi lỗi)
7. `sp[6]` = **PC** (Địa chỉ chính xác của dòng lệnh gây ra lỗi!)
8. `sp[7]` = xPSR

Hãy lấy giá trị hex của **PC** ở vị trí `sp[6]` (word thứ 7 tính từ SP).

### 4. Ánh xạ địa chỉ Hex về dòng code thực tế:
Mở Terminal của hệ thống và chạy công cụ `arm-none-eabi-addr2line`:
```bash
arm-none-eabi-addr2line -e build/app.elf -f [Địa_chỉ_PC_ở_dạng_Hex]
```
Ví dụ:
```bash
arm-none-eabi-addr2line -e build/app.elf -f 0x80003c2
```
Kết quả trả về sẽ hiển thị rõ:
- Tên hàm xảy ra lỗi.
- Tên file và dòng code cụ thể gây crash (ví dụ: `app/src/main.c:8`).

---

## CÂU HỎI BÀI TẬP THỰC HÀNH:
1. Khi chạy `Trigger_HardFault()`, địa chỉ PC ghi nhận được trong Stack Frame của em là bao nhiêu?
2. Khi chạy lệnh `arm-none-eabi-addr2line` với địa chỉ đó, công cụ chỉ ra dòng code nào trong file `main.c`?
3. Nếu CPU bị tràn bộ nhớ Stack (Stack Overflow), thanh ghi SP sẽ có giá trị như thế nào? Sự khác biệt giữa Hard Fault do tràn Stack và Hard Fault do ghi địa chỉ NULL là gì?
