// ================================================================================
// Femboy OS - VGA Framebuffer Header
// ================================================================================

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

// Инициализация VGA framebuffer
void vga_init(void);

// Установка цвета (background, foreground)
void vga_set_color(uint32_t bg, uint32_t fg);

// Вывод текста с цветом
void vga_print(const char *str, uint32_t color_attr);

// Вывод строки с цветовым атрибутом (16-цветный режим)
void vga_print_colored(const char *str, uint8_t bg_index, uint8_t fg_index);

// Печать целого числа
void vga_print_int(int64_t num, uint32_t color_attr);

// Форматированный вывод (упрощенный printf)
void vga_printf(const char *fmt);

// Очистка экрана цветом
void vga_clear(uint32_t color_attr);

// Отладочный вывод
void printk(unsigned long level, const char *fmt, ...);

// Инициализация пастельной темы
void gui_init_pastel_theme(void);

// Инициализация темной кибер темы
void gui_init_cyber_theme(void);

#endif // VGA_H