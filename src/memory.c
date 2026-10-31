// Femboy OS - Memory Management
// Simple memory allocation and initialization

#include <stdio.h>
#include <stdlib.h>

// Memory allocation functions
void* allocate_memory(size_t size) {
    // In a real kernel, this would use mmap, kmalloc, etc.
    // For this demo, we simulate with a simple malloc
    void* ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    return ptr;
}

void free_memory(void* ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

// Simple memory initialization
void init_memory() {
    printf("Initializing memory subsystem...\n");
    // Simulate memory setup
    for (int i = 0; i < 10; i++) {
        // Allocate 64 bytes per memory block
        void* block = allocate_memory(64);
        if (block) {
            printf("  Memory block %d allocated (64 bytes)\n", i);
        }
    }
}
