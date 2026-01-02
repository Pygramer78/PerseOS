# ===== Configuración =====
ARCH        := x86_64
CC          := clang
LD          := ld.lld
ASM         := nasm

CFLAGS      := -ffreestanding -mno-red-zone -Wall -Wextra -Iinclude
LDFLAGS     := -T linker/linker.ld
ASMFLAGS    := -f elf64

BUILD_DIR   := build
ISO_DIR     := iso
KERNEL_ELF  := $(BUILD_DIR)/kernel.elf
ISO_IMAGE   := $(BUILD_DIR)/os.iso

# ===== Fuentes =====
C_SOURCES   := $(shell find kernel include -name "*.c")
ASM_SOURCES := $(shell find boot -name "*.asm")

OBJECTS := \
  $(C_SOURCES:%.c=$(BUILD_DIR)/%.o) \
  $(ASM_SOURCES:%.asm=$(BUILD_DIR)/%.o)

# ===== Targets =====
all: $(ISO_IMAGE)

# ----- Compilar C -----
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ----- Compilar ASM -----
$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(ASM) $(ASMFLAGS) $< -o $@

# ----- Linkear kernel -----
$(KERNEL_ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# ----- Crear ISO -----
$(ISO_IMAGE): $(KERNEL_ELF)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL_ELF) $(ISO_DIR)/boot/
	grub-mkrescue -o $@ $(ISO_DIR)

# ----- Ejecutar -----
run: $(ISO_IMAGE)
	qemu-system-x86_64 -cdrom $(ISO_IMAGE)


# ----- Limpiar -----
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR)/kernel.elf

.PHONY: all run clean
