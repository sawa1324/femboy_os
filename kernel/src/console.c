#include <stdint.h>
#include <stddef.h>
#include "framebuffer.h"
#include "console.h"

#define FONT_WIDTH  8
#define FONT_HEIGHT 8
#define CHAR_SPACING 1

#define CONSOLE_BG 0x10121C
#define CONSOLE_FG 0xE6E6F0

extern const uint8_t font8x8_basic[128][8];

static uint64_t cursor_x = 0;
static uint64_t cursor_y = 0;

static uint32_t foreground = CONSOLE_FG;
static uint32_t background = CONSOLE_BG;

static uint64_t columns = 0;
static uint64_t rows = 0;

static void scroll(void) {
    framebuffer_scroll_up(FONT_HEIGHT, background);

    if (cursor_y > 0) {
        cursor_y--;
    }
}

static void newline(void) {
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= rows) {
        scroll();
    }
}

static void draw_char(char c) {
    uint8_t ch = (uint8_t)c;

    if (ch >= 128) {
        ch = '?';
    }

    const uint8_t *glyph = font8x8_basic[ch];

    uint64_t px = cursor_x * (FONT_WIDTH + CHAR_SPACING);
    uint64_t py = cursor_y * FONT_HEIGHT;

    framebuffer_fill_rect(
        px, py,
        FONT_WIDTH + CHAR_SPACING,
        FONT_HEIGHT,
        background
    );

    for (uint64_t y = 0; y < FONT_HEIGHT; y++) {
        for (uint64_t x = 0; x < FONT_WIDTH; x++) {
            if (glyph[y] & (0x80 >> x)) {
                framebuffer_put_pixel(
                    px + x,
                    py + y,
                    foreground
                );
            }
        }
    }

    cursor_x++;

    if (cursor_x >= columns) {
        newline();
    }
}

void console_init(void) {
    cursor_x = 0;
    cursor_y = 0;

    foreground = CONSOLE_FG;
    background = CONSOLE_BG;

    columns = framebuffer_width() / (FONT_WIDTH + CHAR_SPACING);
    rows = framebuffer_height() / FONT_HEIGHT;

    console_clear();
}

void console_clear(void) {
    cursor_x = 0;
    cursor_y = 0;
    framebuffer_clear(background);
}

void console_putchar(char c) {
    if (columns == 0 || rows == 0) {
        return;
    }

    if (c == '\n') {
        newline();
    } else if (c == '\r') {
        cursor_x = 0;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
        } else if (cursor_y > 0) {
            cursor_y--;
            cursor_x = columns - 1;
        } else {
            return;
        }

        framebuffer_fill_rect(
            cursor_x * (FONT_WIDTH + CHAR_SPACING),
            cursor_y * FONT_HEIGHT,
            FONT_WIDTH + CHAR_SPACING,
            FONT_HEIGHT,
            background
        );
    } else {
        draw_char(c);
    }
}

void console_write(const char *str) {
    if (str == NULL) {
        return;
    }

    while (*str) {
        console_putchar(*str);
        str++;
    }
}

void console_set_color(uint32_t fg, uint32_t bg) {
    foreground = fg;
    background = bg;
}
