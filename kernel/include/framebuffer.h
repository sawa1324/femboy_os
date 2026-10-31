#ifndef FEMBOY_FRAMEBUFFER_H
#define FEMBOY_FRAMEBUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <limine.h>

struct framebuffer {
    struct limine_framebuffer *info;
    uint32_t *pixels;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
};

bool framebuffer_init(struct limine_framebuffer *info);

void framebuffer_clear(uint32_t color);
void framebuffer_put_pixel(uint64_t x, uint64_t y, uint32_t color);
void framebuffer_fill_rect(
    uint64_t x,
    uint64_t y,
    uint64_t width,
    uint64_t height,
    uint32_t color
);

uint64_t framebuffer_width(void);
uint64_t framebuffer_height(void);
void framebuffer_scroll_up(uint64_t pixels, uint32_t color);

#endif
