#include "framebuffer.h"

static struct framebuffer fb;

static uint32_t channel(uint8_t value, uint8_t size, uint8_t shift) {
    uint32_t max = (1u << size) - 1u;
    return (((uint32_t)value * max / 255u) << shift);
}

static uint32_t convert_color(uint32_t rgb) {
    uint8_t r = (rgb >> 16) & 0xFF;
    uint8_t g = (rgb >> 8) & 0xFF;
    uint8_t b = rgb & 0xFF;

    return channel(r, fb.info->red_mask_size, fb.info->red_mask_shift) |
           channel(g, fb.info->green_mask_size, fb.info->green_mask_shift) |
           channel(b, fb.info->blue_mask_size, fb.info->blue_mask_shift);
}

bool framebuffer_init(struct limine_framebuffer *info) {
    if (info == 0 || info->address == 0) {
        return false;
    }

    if (info->memory_model != LIMINE_FRAMEBUFFER_RGB ||
        info->bpp != 32 ||
        info->pitch < info->width * sizeof(uint32_t)) {
        return false;
    }

    fb.info = info;
    fb.pixels = (uint32_t *)info->address;
    fb.width = info->width;
    fb.height = info->height;
    fb.pitch = info->pitch / sizeof(uint32_t);

    return true;
}

void framebuffer_put_pixel(uint64_t x, uint64_t y, uint32_t color) {
    if (x >= fb.width || y >= fb.height || fb.pixels == 0) {
        return;
    }

    fb.pixels[y * fb.pitch + x] = convert_color(color);
}

void framebuffer_clear(uint32_t color) {
    for (uint64_t y = 0; y < fb.height; y++) {
        for (uint64_t x = 0; x < fb.width; x++) {
            framebuffer_put_pixel(x, y, color);
        }
    }
}

void framebuffer_fill_rect(
    uint64_t x,
    uint64_t y,
    uint64_t width,
    uint64_t height,
    uint32_t color
) {
    if (x >= fb.width || y >= fb.height) {
        return;
    }

    if (width > fb.width - x) {
        width = fb.width - x;
    }

    if (height > fb.height - y) {
        height = fb.height - y;
    }

    for (uint64_t py = y; py < y + height; py++) {
        for (uint64_t px = x; px < x + width; px++) {
            framebuffer_put_pixel(px, py, color);
        }
    }
}
uint64_t framebuffer_width(void) {
    return fb.width;
}

uint64_t framebuffer_height(void) {
    return fb.height;
}

void framebuffer_scroll_up(uint64_t pixels, uint32_t color) {
    if (pixels >= fb.height) {
        framebuffer_clear(color);
        return;
    }

    for (uint64_t y = 0; y < fb.height - pixels; y++) {
        for (uint64_t x = 0; x < fb.width; x++) {
            fb.pixels[y * fb.pitch + x] =
                fb.pixels[(y + pixels) * fb.pitch + x];
        }
    }

    framebuffer_fill_rect(
        0,
        fb.height - pixels,
        fb.width,
        pixels,
        color
    );
}
