.PHONY: all help kernel clean

all: help

help:
	@echo "Femboy OS build system"
	@echo ""
	@echo "Targets:"
	@echo "  make kernel - Build the kernel"
	@echo "  make clean  - Remove build artifacts"
	@echo "  make help   - Show this help"

kernel:
	$(MAKE) -C kernel

clean:
	$(MAKE) -C kernel clean
