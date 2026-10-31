#ifndef FEMBOY_CONSOLE_H
#define FEMBOY_CONSOLE_H

#include <stdint.h>

void console_init(void);
void console_clear(void);
void console_putchar(char c);
void console_write(const char *str);
void console_set_color(uint32_t foreground, uint32_t background);

#endif
