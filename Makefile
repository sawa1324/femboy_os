PROJECT_NAME := femboy-os
ISO_DIR := iso/femboy-os
ISO := $(PROJECT_NAME)-console.iso

KERNEL := kernel/build/kernel
LIMINE := tools/limine-binary/limine-binary/limine

QEMU := qemu-system-x86_64
QEMU_MEMORY := 2G
QEMU_MACHINE := q35

.PHONY: all help kernel iso run clean

all: help

help:
	@echo "Femboy OS build system"
	@echo ""
	@echo "Targets:"
	@echo "  make kernel - Build the kernel"
	@echo "  make iso    - Build bootable ISO"
	@echo "  make run    - Build ISO and launch QEMU"
	@echo "  make clean  - Remove build artifacts"

kernel:
	$(MAKE) -C kernel

iso: kernel
	@test -x "$(LIMINE)" || (echo "Limine installer not found"; exit 1)
	@test -f "$(ISO_DIR)/EFI/BOOT/BOOTX64.EFI" || (echo "UEFI bootloader not found"; exit 1)
	@test -f "$(ISO_DIR)/boot/limine/limine-bios-cd.bin" || (echo "BIOS bootloader not found"; exit 1)
	@echo "Copying kernel..."
	cp "$(KERNEL)" "$(ISO_DIR)/boot/kernel"
	@echo "Creating ISO..."
	xorriso -as mkisofs -R -r -J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		-hfsplus -apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		"$(ISO_DIR)" -o "$(ISO)"
	@echo "Installing Limine BIOS stages..."
	"$(LIMINE)" bios-install "$(ISO)"
	@echo "ISO created: $(ISO)"

run: iso
	$(QEMU) -M $(QEMU_MACHINE) -m $(QEMU_MEMORY) \
		-cdrom "$(ISO)" -boot d

clean:
	$(MAKE) -C kernel clean
	rm -f "$(ISO)"
	rm -f "$(ISO_DIR)/boot/kernel"
