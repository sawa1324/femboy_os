// ================================================================================
// Femboy OS - Console / VGA Framebuffer Driver
// ================================================================================
// Базовый драйвер для работы с VGA framebuffer для вывода графики и текста
// Поддержка 24bpp RGB + базовые функции отрисовки

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "colors.h"
#include "../include/vga.h"

// ============================================================================
// Константы VGA Framebuffer
// ============================================================================

#define VGA_FRAMEBUFFER_ADDR  0xB8000    // Стандартный адрес VGA text mode
#define VGA_FRAMEBUFFER_24bpp 0xF0000000 // Адрес 24bpp RGB framebuffer (Llvtm)

#define VGA_WIDTH             800        // Ширина экрана в пикселях
#define VGA_HEIGHT            600        // Высота экрана в пикселях
#define VGA_BUFFER_SIZE       (VGA_WIDTH * VGA_HEIGHT)

// ============================================================================
// Глобальный указатель на framebuffer
// ============================================================================

static volatile uint32_t *vga_fb = NULL;     // 24bpp framebuffer
static uint32_t current_color_bg = COLOR_PASTEL_WHITE.b; // Текущий background
static uint32_t current_color_fg = COLOR_PASTEL_DARK_GRAY.b;   // Текущий foreground

// ============================================================================
// Инициализация VGA framebuffer
// ============================================================================

void vga_init(void) {
    // Записываем адрес framebuffer в LVTM register (если поддерживается)
    // Или используем стандартный 16-цветный режим для совместимости
    
    // Для простоты начнем с текстового режима 80x25
    uint16_t *text_buf = (uint16_t *)VGA_FRAMEBUFFER_ADDR;
    
    // Очищаем экран белым цветом
    for (int i = 0; i < 80 * 25; i++) {
        text_buf[i] = COLOR_PASTEL_WHITE | (COLOR_PASTEL_DARK_GRAY << 8);
    }
    
    vga_fb = (volatile uint32_t *)text_buf; // Используем как pointer к packed данным
    
    printk(KERN_INFO "Femboy OS: VGA framebuffer initialized (80x25 text mode)\n");
}

// ============================================================================
// Установить цвет экрана
// ============================================================================

void vga_set_color(uint32_t bg, uint32_t fg) {
    current_color_bg = bg;
    current_color_fg = fg;
}

// ============================================================================
// Вывод текста на экран (с поддержкой цветов)
// ============================================================================

void vga_print(const char *str, uint32_t color_attr) {
    if (!vga_fb) return;
    
    // В текстовом режиме 16-цветный
    uint16_t *text_buf = (uint16_t *)vga_fb;
    
    while (*str) {
        char c = *str++;
        
        if (c == '\n') {
            // Переход на новую строку
            int current_pos = (text_buf - (uint16_t *)vga_fb);
            text_buf[current_pos / 80] = 0; // Сброс цвета (можно оптимизировать)
            text_buf[(current_pos + 1) / 80] = COLOR_PASTEL_WHITE | (COLOR_PASTEL_DARK_GRAY << 8);
        } else if (c >= 32 && c <= 126) { // Печатные символы
            uint16_t attr = color_attr;
            text_buf[text_buf / 80] = attr;
            
            // Получаем символ из font (упрощенно - ASCII код как индекс)
            char glyph = c;
        } else if (c == '\t') {
            // Табуляция
            int current_pos = (text_buf - (uint16_t *)vga_fb);
            int cols = current_pos % 80;
            int next_tab = ((cols / 8) + 1) * 8;
            for (int j = cols; j < next_tab; j++) {
                text_buf[j / 80] = COLOR_PASTEL_WHITE | (COLOR_PASTEL_DARK_GRAY << 8);
            }
        }
    }
}

// ============================================================================
// Вывод строки с цветом фона и переднего плана
// ============================================================================

void vga_print_colored(const char *str, uint8_t bg_index, uint8_t fg_index) {
    if (!vga_fb) return;
    
    uint16_t attr = (bg_index << 8) | fg_index;
    vga_print(str, attr);
}

// ============================================================================
// Печать целого числа
// ============================================================================

void vga_print_int(int64_t num, uint32_t color_attr) {
    char buffer[32];
    int len = 0;
    bool negative = num < 0;
    
    if (negative) num = -num;
    
    // Преобразуем в строку
    do {
        buffer[len++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);
    
    if (negative) buffer[len++] = '-';
    
    // Выводим в обратном порядке
    for (int i = len - 1; i >= 0; i--) {
        vga_print(&buffer[i], color_attr);
    }
}

// ============================================================================
// Печать с переносом строки
// ============================================================================

void vga_printf(const char *fmt) {
    // Простая реализация без full printf - только для базовой отладки
    printk(KERN_INFO fmt);
}

// ============================================================================
// Очистить экран цветом
// ============================================================================

void vga_clear(uint32_t color_attr) {
    if (!vga_fb) return;
    
    uint16_t *text_buf = (uint16_t *)vga_fb;
    for (int i = 0; i < 80 * 25; i++) {
        text_buf[i] = color_attr;
    }
}

// ============================================================================
// Отладочный вывод (как printf)
// ============================================================================

void printk(unsigned long level, const char *fmt, ...) {
    // Базовый логирование - в полной версии будет ring buffer
    if (!vga_fb) return;
    
    vga_print(fmt, COLOR_PASTEL_WHITE | (COLOR_PASTEL_DARK_GRAY << 8));
}

// ============================================================================
// Инициализация пастельной темы
// ============================================================================

void gui_init_pastel_theme(void) {
    uint32_t bg = (uint32_t)COLOR_PASTEL_WHITE.b;
    uint32_t fg = (uint32_t)COLOR_PASTEL_DARK_GRAY.b;
    
    vga_set_color(bg, fg);
    vga_clear((bg << 8) | fg);
    
    printk(KERN_INFO "Femboy OS: Pastel theme initialized\n");
}

// ============================================================================
// Инициализация темной кибер темы
// ============================================================================

void gui_init_cyber_theme(void) {
    uint32_t bg = (uint32_t)COLOR_CYBER_BLACK.b;
    uint32_t fg = (uint32_t)COLOR_CYBER_GREEN.b;
    
    vga_set_color(bg, fg);
    vga_clear((bg << 8) | fg);
    
    printk(KERN_INFO "Femboy OS: Cyber dark theme initialized\n");
}
