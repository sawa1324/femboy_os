#include <stdint.h>
#include "timer.h"

#define PIT_DIVISOR 1193

static uint16_t previous = 0;
static uint64_t elapsed_counts = 0;

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" :: "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static uint16_t read_counter(void) {
    outb(0x43, 0x00);

    uint8_t low = inb(0x40);
    uint8_t high = inb(0x40);

    return (uint16_t)(low | ((uint16_t)high << 8));
}

void timer_init(void) {
    outb(0x43, 0x34);
    outb(0x40, PIT_DIVISOR & 0xFF);
    outb(0x40, PIT_DIVISOR >> 8);

    elapsed_counts = 0;
    previous = read_counter();
}

uint64_t timer_milliseconds(void) {
    uint16_t current = read_counter();

    if (previous >= current) {
        elapsed_counts += previous - current;
    } else {
        elapsed_counts += previous + (PIT_DIVISOR - current);
    }

    previous = current;

    return elapsed_counts * 1000 / 1193182;
}
