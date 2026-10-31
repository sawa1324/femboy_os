// Femboy OS - Memory Header
// Memory management interface

#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>

// Allocate memory of given size
void* allocate_memory(size_t size);

// Free allocated memory
void free_memory(void* ptr);

// Initialize memory subsystem
void init_memory();

#endif // MEMORY_H
