CC        := arm-none-eabi-gcc
OBJCOPY   := arm-none-eabi-objcopy
SIZE      := arm-none-eabi-size

TARGET    := app
BUILD     := build

INCS      := -Iapp/inc -Ibsp/inc -Icore/inc

SRCS      := app/src/main.c \
             bsp/src/bsp.c \
             bsp/src/gpio.c \
             bsp/src/systick.c \
             bsp/src/button.c \
             core/src/startup_stm32f411xe.c \
             core/src/system_stm32f4xx.c

CFLAGS    := -mcpu=cortex-m4 \
             -mthumb \
             -mfloat-abi=hard \
             -mfpu=fpv4-sp-d16 \
             -g3 \
             -O2 \
             -Wall \
             -Wextra \
             -ffunction-sections \
             -fdata-sections \
             -fno-builtin \
             -fno-tree-loop-distribute-patterns \
             -DSTM32F411xE \
             $(INCS)

LDFLAGS   := -Tf411re.ld \
             -nostartfiles \
             -nostdlib \
             -Wl,--gc-sections \
             -Wl,-Map=$(BUILD)/$(TARGET).map \
             -Wl,--cref

OBJS      := $(patsubst %.c,$(BUILD)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
else
    MKDIR = mkdir -p $(1)
endif

.PHONY: all build flash size clean

all: build size

build: $(BUILD)/$(TARGET).hex $(BUILD)/$(TARGET).bin

$(BUILD)/$(TARGET).elf: $(OBJS)
	@echo "[LD] $@"
	@$(CC) $(OBJS) $(LDFLAGS) -o $@

$(BUILD)/$(TARGET).bin: $(BUILD)/$(TARGET).elf
	@echo "[OBJCOPY] $@"
	@$(OBJCOPY) -O binary $< $@

$(BUILD)/$(TARGET).hex: $(BUILD)/$(TARGET).elf
	@echo "[OBJCOPY] $@"
	@$(OBJCOPY) -O ihex $< $@

$(BUILD)/%.o: %.c
	@echo "[CC] $<"
	@$(call MKDIR,$(dir $@))
	@$(CC) $(CFLAGS) -c $< -o $@

flash: $(BUILD)/$(TARGET).elf
	@echo "[FLASH] $<"
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program $< verify reset exit"

size: $(BUILD)/$(TARGET).elf
	@echo "[SIZE] $<"
	@$(SIZE) $<

clean:
	@echo "[CLEAN]"
ifeq ($(OS),Windows_NT)
	@if exist "$(subst /,\,$(BUILD))" rmdir /S /Q "$(subst /,\,$(BUILD))"
else
	@rm -rf $(BUILD)
endif

