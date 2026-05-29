# NHIỆM VỤ: Viết Makefile cho L1_Blink
# ======================================
#
# KIẾN THỨC CẦN HỌC TRƯỚC:
#   - GNU Make cơ bản: target, prerequisite, recipe, variables, pattern rules
#     https://www.gnu.org/software/make/manual/make.html
#   - arm-none-eabi-gcc: cross-compiler cho ARM, các flag quan trọng
#   - arm-none-eabi-objcopy: chuyển .elf → .bin / .hex
#   - arm-none-eabi-size: xem kích thước .text .data .bss
#   - OpenOCD: flash firmware qua ST-Link
#
# CÂU HỎI CẦN TRẢ LỜI TRƯỚC KHI CODE:
#   1. -mcpu=cortex-m4 nghĩa là gì?
#		cấu hình thiết bị
#   2. -mthumb là gì? Tại sao dùng Thumb thay vì ARM instruction set?
#		dịch mã C thành tập lệnh thumb thay vì ARM. Dùng thumb vì nhân Cortex-M không hỗ trợ ARM 32bit mà chỉ chạy được thumb
#   3. -mfloat-abi=hard và -mfpu=fpv4-sp-d16 liên quan đến gì?
#		-mfpu=fpv4-sp-d16 có tác dụng khai báo phần cứng FPU có trách nhiệm tính toán số thực. Còn -mfloat-abi=hard là chế độ cho phép trình biên dịch truyền các hàm trực tiếp vào thanh gh của FPU để tính toán nhanh chóng
#   4. -nostdlib làm gì? Tại sao bare-metal cần flag này?
#		ra lệnh cho Linker không tự động liên kết các thư viện C và các file khởi động hệ thống mặc định. Khi lập trình bare-metal thì board không có săn hệ điều hành (OS) vì vậy nếu k có flag này thì dễ gây tràn bộ nhớ, xung đột khởi tạo
#   5. -ffunction-sections và --gc-sections phối hợp với nhau thế nào?
# 		-ffunction-sections sẽ ép compiler chia nhỏ từng hàm vào 1 phân vùng độc lập trong .text. Sau đó --gc-sections sẽ dùng linker để quét các hàm dùng tới thì giữ lại còn nhưng hàm không dùng tới thì bỏ ra khỏi file .elf để tối ưu không gian nhớ.
#   6. -g3 và -O0 dùng để làm gì?
#		-g3 thêm đầy đủ thông tin debug vào file thực thi để dùng với debugger. -O0 tắt tối ưu hóa code giúp debug dễ và chính xác theo source code.
#   7. Sự khác nhau giữa .elf, .bin, .hex?
#		.elf chứa code + symbol + debug info, dùng để debug và nạp qua debugger..bin là binary thuần, nhỏ gọn, thường dùng để flash firmware.hex là dạng text Intel HEX có địa chỉ memory và checksum.
#   8. Pattern rule $(BUILD)/%.o: %.c hoạt động thế nào?
# 		Đây là pattern rule trong Makefile để tạo file .o từ file .c.
# NHIỆM VỤ PHẢI LÀM:

# Minimal Makefile for L1_Blink

CC        := arm-none-eabi-gcc
OBJCOPY   := arm-none-eabi-objcopy
SIZE      := arm-none-eabi-size

TARGET    := blink_led
BUILD     := build
SRCS      := src/main.c src/startup_f4.c
LD_SCRIPT := linker/f411re.ld

CFLAGS := \
	-mcpu=cortex-m4 \
	-mthumb \
	-mfloat-abi=hard \
	-mfpu=fpv4-sp-d16 \
	-g3 \
	-O0 \
	-Wall \
	-Wextra \
	-Werror \
	-ffunction-sections \
	-fdata-sections \
	-Iinc

LDFLAGS := \
	-T$(LD_SCRIPT) \
	-nostartfiles \
	-nostdlib \
	-Wl,--gc-sections \
	-Wl,-Map=$(BUILD)/$(TARGET).map \
	-Wl,--cref

OBJS := $(patsubst src/%.c,$(BUILD)/%.o,$(SRCS))

.PHONY: all build flash size clean help

all: build size

build: $(BUILD)/$(TARGET).hex

$(BUILD)/$(TARGET).elf: $(OBJS)
	@echo "[LD] $@"
	$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	@echo "[OBJCOPY] $@"
	$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	@echo "[OBJCOPY] $@"
	$(OBJCOPY) -O ihex $< $@

$(BUILD)/%.o: src/%.c | $(BUILD)/.dir
	@echo "[CC] $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/.dir:
ifeq ($(OS),Windows_NT)
	@if not exist "$(subst /,\,$(dir $(BUILD)/.dir))" mkdir "$(subst /,\,$(dir $(BUILD)/.dir))"
else
	@mkdir -p $(dir $(BUILD)/.dir)
endif

flash: $(BUILD)/$(TARGET).elf
	@echo "[FLASH] $<"
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $< verify reset exit"

size: $(BUILD)/$(TARGET).elf
	@echo "[SIZE] $<"
	$(SIZE) $<

clean:
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BUILD))" rmdir /S /Q "$(subst /,\,$(BUILD))"
else
	rm -rf $(BUILD)
endif


