#include <stdint.h>
#include <stddef.h>
#include "framebuffer.h"
#include "console.h"
#include "timer.h"

#define FONT_WIDTH 8
#define FONT_HEIGHT 8
#define CHAR_SPACING 1

struct theme_colors {
    uint32_t background;
    uint32_t foreground;
    uint32_t status_background;
    uint32_t status_foreground;
    uint32_t cursor;
};

static const struct theme_colors dark_theme = {
    0x10121C,
    0xE6E6F0,
    0x1B1E2D,
    0xD88BC7,
    0xD88BC7
};

static const struct theme_colors pastel_theme = {
    0xF8EAF1,
    0x70465D,
    0xF1D5E2,
    0xB65F89,
    0xD875A5
};

extern const uint8_t font8x8_basic[128][8];

static struct theme_colors colors;
static enum console_theme current_theme = CONSOLE_THEME_DARK;

static uint64_t cursor_x = 0;
static uint64_t cursor_y = 0;
static uint64_t columns = 0;
static uint64_t rows = 0;
static uint64_t usable_rows = 0;

static uint32_t foreground;
static uint32_t background;

static uint64_t last_line = UINT64_MAX;
static uint64_t last_column = UINT64_MAX;
static int cursor_drawn = 0;
static int cursor_phase = 0;

static void draw_glyph_at(
    uint64_t x,
    uint64_t y,
    char c,
    uint32_t fg,
    uint32_t bg
) {
    uint8_t ch = (uint8_t)c;
    if (ch >= 128)
        ch = '?';

    const uint8_t *glyph = font8x8_basic[ch];

    uint64_t px = x * (FONT_WIDTH + CHAR_SPACING);
    uint64_t py = y * FONT_HEIGHT;

    framebuffer_fill_rect(
        px, py,
        FONT_WIDTH + CHAR_SPACING,
        FONT_HEIGHT,
        bg
    );

    for (uint64_t gy = 0; gy < FONT_HEIGHT; gy++) {
        for (uint64_t gx = 0; gx < FONT_WIDTH; gx++) {
            if (glyph[gy] & (0x80 >> gx))
                framebuffer_put_pixel(px + gx, py + gy, fg);
        }
    }
}

static void draw_number(uint64_t x, uint64_t y, uint64_t value) {
    char digits[20];
    size_t count = 0;

    if (value == 0) {
        draw_glyph_at(x, y, '0',
                      colors.status_foreground,
                      colors.status_background);
        return;
    }

    while (value > 0 && count < sizeof(digits)) {
        digits[count++] = '0' + (value % 10);
        value /= 10;
    }

    while (count > 0) {
        draw_glyph_at(x++, y, digits[--count],
                      colors.status_foreground,
                      colors.status_background);
    }
}

static void draw_status(void) {
    if (rows == 0)
        return;

    uint64_t y = rows - 1;

    framebuffer_fill_rect(
        0, y * FONT_HEIGHT,
        framebuffer_width(), FONT_HEIGHT,
        colors.status_background
    );

    draw_glyph_at(1, y, 'L', colors.status_foreground, colors.status_background);
    draw_glyph_at(2, y, 'I', colors.status_foreground, colors.status_background);
    draw_glyph_at(3, y, 'N', colors.status_foreground, colors.status_background);
    draw_glyph_at(4, y, 'E', colors.status_foreground, colors.status_background);

    draw_glyph_at(12, y, 'C', colors.status_foreground, colors.status_background);
    draw_glyph_at(13, y, 'O', colors.status_foreground, colors.status_background);
    draw_glyph_at(14, y, 'L', colors.status_foreground, colors.status_background);
    draw_glyph_at(15, y, 'U', colors.status_foreground, colors.status_background);
    draw_glyph_at(16, y, 'M', colors.status_foreground, colors.status_background);
    draw_glyph_at(17, y, 'N', colors.status_foreground, colors.status_background);

    last_line = UINT64_MAX;
    last_column = UINT64_MAX;
}

static void update_status_position(void) {
    if (rows == 0)
        return;

    uint64_t y = rows - 1;

    if (last_line == cursor_y && last_column == cursor_x)
        return;

    framebuffer_fill_rect(6 * 9, y * FONT_HEIGHT, 5 * 9,
                          FONT_HEIGHT, colors.status_background);
    framebuffer_fill_rect(19 * 9, y * FONT_HEIGHT, 5 * 9,
                          FONT_HEIGHT, colors.status_background);

    draw_number(6, y, cursor_y + 1);
    draw_number(19, y, cursor_x + 1);

    last_line = cursor_y;
    last_column = cursor_x;
}

static void hide_cursor(void) {
    if (!cursor_drawn)
        return;

    framebuffer_fill_rect(
        cursor_x * (FONT_WIDTH + CHAR_SPACING),
        cursor_y * FONT_HEIGHT,
        FONT_WIDTH + CHAR_SPACING,
        FONT_HEIGHT,
        background
    );

    cursor_drawn = 0;
}

static void scroll(void) {
    framebuffer_scroll_up(FONT_HEIGHT, background);

    framebuffer_fill_rect(
        0,
        usable_rows * FONT_HEIGHT,
        framebuffer_width(),
        FONT_HEIGHT,
        colors.status_background
    );

    if (cursor_y > 0)
        cursor_y--;
}

static void newline(void) {
    cursor_x = 0;
    cursor_y++;

    if (cursor_y >= usable_rows)
        scroll();
}

static void draw_char(char c) {
    draw_glyph_at(cursor_x, cursor_y, c, foreground, background);

    cursor_x++;

    if (cursor_x >= columns)
        newline();
}

void console_init(void) {
    cursor_x = 0;
    cursor_y = 0;

    columns = framebuffer_width() / (FONT_WIDTH + CHAR_SPACING);
    rows = framebuffer_height() / FONT_HEIGHT;
    usable_rows = rows > 1 ? rows - 1 : rows;

    cursor_drawn = 0;
    cursor_phase = 0;
    last_line = UINT64_MAX;
    last_column = UINT64_MAX;

    console_set_theme(CONSOLE_THEME_PASTEL);
    console_clear();
}

void console_clear(void) {
    hide_cursor();

    cursor_x = 0;
    cursor_y = 0;

    framebuffer_clear(background);
    draw_status();
}

void console_putchar(char c) {
    if (columns == 0 || usable_rows == 0)
        return;

    hide_cursor();

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

    update_status_position();
}

void console_write(const char *str) {
    if (str == NULL)
        return;

    while (*str)
        console_putchar(*str++);
}

void console_set_color(uint32_t fg, uint32_t bg) {
    foreground = fg;
    background = bg;
}

void console_set_theme(enum console_theme theme) {
    hide_cursor();

    if (theme == CONSOLE_THEME_PASTEL) {
        colors = pastel_theme;
        current_theme = CONSOLE_THEME_PASTEL;
    } else {
        colors = dark_theme;
        current_theme = CONSOLE_THEME_DARK;
    }

    foreground = colors.foreground;
    background = colors.background;

    cursor_phase = 0;
    last_line = UINT64_MAX;
    last_column = UINT64_MAX;

    framebuffer_clear(background);
    draw_status();
    update_status_position();
}

enum console_theme console_get_theme(void) {
    return current_theme;
}

void console_update_cursor(void) {
    uint64_t ms = timer_milliseconds();
    int phase = (ms / 500) & 1;

    update_status_position();

    if (phase == cursor_phase)
        return;

    hide_cursor();
    cursor_phase = phase;

    if (cursor_phase) {
        uint64_t px = cursor_x * (FONT_WIDTH + CHAR_SPACING);
        uint64_t py = cursor_y * FONT_HEIGHT;

        framebuffer_fill_rect(
            px, py + 1, 2, FONT_HEIGHT - 2, colors.cursor
        );

        cursor_drawn = 1;
    }
}

void console_set_cursor(uint64_t x, uint64_t y) {
    hide_cursor();

    if (x >= columns)
        x = columns - 1;

    if (y >= usable_rows)
        y = usable_rows - 1;

    cursor_x = x;
    cursor_y = y;

    update_status_position();
}
