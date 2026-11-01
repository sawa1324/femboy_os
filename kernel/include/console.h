#ifndef FEMBOY_CONSOLE_H
#define FEMBOY_CONSOLE_H

#include <stdint.h>

enum console_theme {
    CONSOLE_THEME_DARK,
    CONSOLE_THEME_PASTEL
};

void console_init(void);
void console_clear(void);
void console_putchar(char c);
void console_write(const char *str);
void console_set_color(uint32_t foreground, uint32_t background);
void console_update_cursor(void);
void console_set_cursor(uint64_t x, uint64_t y);

void console_set_theme(enum console_theme theme);
enum console_theme console_get_theme(void);

#endif
