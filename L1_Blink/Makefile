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


