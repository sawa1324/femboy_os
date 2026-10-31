// Femboy OS - Minimal Linux Kernel
// Simple bootloader and kernel for QEMU

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Simple kernel entry point
void kernel_main(void) {
    printf("Femboy OS v0.1.0 - Booted!\n");
    printf("Hello, Femboy OS!\n");
    // HLT - halt the CPU (kernel is now running)
    __asm__("hlt");
}

// Simple memory management
void init_memory(void) {
    printf("Initializing memory...\n");
    // In a real kernel, this would set up page tables, etc.
}

// Simple bootloader stub
void bootloader(void) {
    printf("Bootloader: Femboy OS v0.1.0\n");
    // Jump to kernel
    __asm__("jmp 0x80000000");
}

int main(int argc, char* argv[]) {
    // Early initialization
    init_memory();
    
    // Bootloader code
    bootloader();
    
    // Kernel entry
    kernel_main();
    
    // HLT - halt the CPU (kernel is now running)
    __asm__("hlt");
    
    return 0;
}
