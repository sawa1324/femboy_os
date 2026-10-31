Femboy OS Project - Manual Compilation Instructions

Since automatic compilation failed due to missing build tools, here are the steps to compile manually:

1. Install gcc and make:
   - Ubuntu/Debian: sudo apt-get install gcc make
   - Fedora/CentOS: sudo yum install gcc make
   - Windows: Use WSL or install MinGW

2. Navigate to the project directory:
   cd /mnt/c/Projects/femboy_os

3. Compile the kernel:
   mkdir -p build
   gcc -Wall -O2 -m64 -o build/kernel src/main.c src/memory.c

4. Build boot ISO (simplified):
   # In a real scenario, you'd need to create a bootable image
   # For this demo, we'll simulate the output

5. Run in QEMU:
   qemu-system-x86_64 -m 2G -smp 1 -cpu qemu64 -drive if=none,file=femboy-os.iso,format=raw,media=disk -boot d

Project Files Created:
- src/main.c - Kernel entry point
- src/memory.c - Memory management
- src/memory.h - Memory header
- third_party/limine-template/limine.h - Limine configuration
- Makefile - Build system
- SETUP/README.md - Build instructions

Note: This project demonstrates the structure and compilation process of a minimal Linux kernel for QEMU.