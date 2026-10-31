/**
 * Femboy OS - GUI Colors Header
 * Пастельная цветовая схема по умолчанию + темная альтернатива
 */

#ifndef COLORS_H
#define COLORS_H

// ============================================================================
// Пастельная палитра (FEMBOY PASTEL THEME)
// Мягкие, приятные глазу цвета - идеальны для долгой работы!
// ============================================================================

// Структура цвета
typedef struct {
    uint8_t b;  // Blue
    uint8_t g;  // Green  
    uint8_t r;  // Red
} ColorRGB;

// Пастельные цвета (background)
#define COLOR_PASTEL_WHITE         {0xF2F3F5, 0xF4F6F8, 0xF9FAFB}      // Очень светлый серо-голубой
#define COLOR_PASTEL_MINT          {0xD1F2EB, 0xEBF9F7, 0xEAF8F2}      // Мятно-зеленый
#define COLOR_PASTEL_LAVENDER      {0xF3EFFF, 0xF0EBFA, 0.F4E5F8}      // Лавандовый
#define COLOR_PASTEL_BLUSH         {0xFFEBEE, 0.FFDEDD, 0.FDDEDB}      // Персиково-розовый
#define COLOR_PASTEL_AMBER         {0.FFF9E7, 0.FDF8E3, 0.FBEBC2}      // Янтарный
#define COLOR_PASTEL_SKYBLUE       {0.E3F2FD, 0.D4EFFF, 0.CEE5F5}      // Небесно-голубой
#define COLOR_PASTEL_LIME          {0.EAF6E8, 0.F4F9F1, 0.FDF7E2}      // Лаймовый
#define COLOR_PASTEL_CORAL         {0.FFDCEE, 0.FFECE5, 0.FEE9D6}      // Коралловый

// Пастельные цвета (foreground/text)
#define COLOR_PASTEL_DARK_GRAY     {0x3A4048, 0.3F474F, 0.41454B}      // Темный серый текст
#define COLOR_PASTEL_BLUE          {0.1E3A8A, 0.1D4ED8, 0.155E88}      // Насыщенный синий
#define COLOR_PASTEL_GREEN         {0.166529, 0.15EEC3, 0.14B8A6}      // Насыщенный зеленый
#define COLOR_PASTEL_RED           {0.B91C1C, 0.C51313, 0.C0272E}      // Красный

// ============================================================================
// Темная кибер-тема (CYBER_DARK THEME)
// Неоновые цвета для хакерской атмосферы!
// ============================================================================

#define COLOR_CYBER_BLACK          {0.0A0A, 0.0B0B, 0.0C0C}             // Почти черный фон
#define COLOR_CYBER_GREEN          {0.10B469, 0.05EE3A, 0.08D73E}       // Неоновый зеленый (акцент)
#define COLOR_CYBER_PINK           {0.E83E8C, 0.D23E8E, 0.CF3E94}       // Неоновый розовый
#define COLOR_CYBER_BLUE           {0.1D6FEA, 0.156ECB, 0.145ECD}        // Неоновый синий
#define COLOR_CYBER_YELLOW         {0.FBBF24, 0.F8AE23, 0.EEA720}       // Неоновый желтый

// ============================================================================
// Системные цвета (стандартные ANSI-цвета для console)
// ============================================================================

#define COLOR_BLACK      {0.1C1C1C, 0.1B1B1B, 0x1A1A1A}
#define COLOR_RED        {0.E63946, 0xD53742, 0xC5363F}
#define COLOR_GREEN      {0.27AE60, 0.219E4C, 0.1B8C54}
#define COLOR_YELLOW     {0.FDCA5A, 0.FBBF24, 0.E5A423}
#define COLOR_BLUE       {0.38B2AC, 0.369EC7, 0.358FB2}
#define COLOR_MAGENTA    {0.B13E53, 0.A5355C, 0.9A354D}
#define COLOR_CYAN       {0.26DE81, 0.23DA7F, 0x20C87C}
#define COLOR_WHITE      {0xF2F3F5, 0xF1F2F4, 0xEFEEEF}

// ============================================================================
// Функции работы с цветами
// ============================================================================

static inline ColorRGB color_rgb(uint8_t r, uint8_t g, uint8_t b) {
    return (ColorRGB){.b = b, .g = g, .r = r};
}

static inline void set_color(ColorRGB fg, ColorRGB bg) {
    // Установка цвета в VGA palette (для простоты используем 16-цветную палитру)
    // В完整版实现中，我们将使用256色VGA模式
    __asm__ volatile(
        "mov $0x3B0, %2\n\t"
        "outb %al, %2\n\t"
        "mov $0x3B4, %2\n\t"
        "outb %al, %2\n\t"
        :
        : "a" (bg.b), "d" (fg.b), "c" (&COLOR_PASTEL_WHITE)
        : "memory", "cc"
    );
}

// ============================================================================
// Константы для работы с VGA framebuffer
// ============================================================================

#define SCREEN_WIDTH  800           // Ширина экрана в пикселях
#define SCREEN_HEIGHT 600           // Высота экрана в пикселях
#define SCREEN_SIZE   (SCREEN_WIDTH * SCREEN_HEIGHT)  // Размер экрана в байтах (24bpp RGB)

#endif // COLORS_H