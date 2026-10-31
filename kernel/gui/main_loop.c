// ================================================================================
// Femboy OS - Main Loop / GUI System
// ================================================================================
// Основной цикл работы GUI системы с поддержкой:
// - Консольного вывода
// - Базовой графики
// - Пастельной и кибер тем

#include <stdint.h>
#include <stddef.h>
#include "colors.h"
#include "../include/vga.h"

// ============================================================================
// Глобальное состояние GUI
// ============================================================================

static volatile int gui_initialized = 0;
static volatile uint32_t current_theme = 0; // 0 = pastel, 1 = cyber

// ============================================================================
// Основной цикл GUI
// ============================================================================

void main_loop(void) {
    if (!gui_initialized) return;
    
    printk(KERN_INFO "Femboy OS: Entering main loop...\n");
    
    // Здесь будет основной цикл обработки событий
    // Пока просто бесконечный цикл с ожиданием
    while (1) {
        asm volatile("hlt");
    }
}

// ============================================================================
// Инициализация GUI системы
// ============================================================================

void gui_init(void) {
    if (gui_initialized) {
        printk(KERN_INFO "Femboy OS: GUI already initialized\n");
        return;
    }
    
    // Инициализация VGA framebuffer
    vga_init();
    
    // Установка пастельной темы по умолчанию
    gui_init_pastel_theme();
    
    gui_initialized = 1;
    current_theme = 0;
    
    printk(KERN_INFO "Femboy OS: GUI initialized with pastel theme\n");
}

// ============================================================================
// Переключение на пастельную тему
// ============================================================================

void switch_to_pastel_theme(void) {
    if (!gui_initialized) return;
    
    current_theme = 0;
    gui_init_pastel_theme();
    
    printk(KERN_INFO "Femboy OS: Switched to pastel theme\n");
}

// ============================================================================
// Переключение на кибер тему
// ============================================================================

void switch_to_cyber_theme(void) {
    if (!gui_initialized) return;
    
    current_theme = 1;
    gui_init_cyber_theme();
    
    printk(KERN_INFO "Femboy OS: Switched to cyber dark theme\n");
}

// ============================================================================
// Печать приветственного экрана
// ============================================================================

void print_boot_screen(void) {
    if (!gui_initialized) return;
    
    // Заголовок
    vga_print("=========================================\n", 
              (uint32_t)COLOR_PASTEL_WHITE.b | ((uint32_t)COLOR_PASTEL_BLUE.b << 8));
    vga_print("         FEMBOY OS 1.0                  \n", 
              (uint32_t)COLOR_PASTEL_WHITE.b | ((uint32_t)COLOR_PASTEL_BLUE.b << 8));
    vga_print("=========================================\n", 
              (uint32_t)COLOR_PASTEL_WHITE.b | ((uint32_t)COLOR_PASTEL_BLUE.b << 8));
    
    // Информация о системе
    vga_print("\n");
    vga_print("Welcome to Femboy OS!\n", COLOR_PASTEL_MINT);
    vga_print("A pastel-themed open-source operating system\n", COLOR_PASTEL_LAVENDER);
    vga_print("built with love and care. 💕\n", COLOR_PASTEL_BLUSH);
    
    vga_print("\n");
    vga_print("System Information:\n", COLOR_PASTEL_SKYBLUE);
    vga_print("  Architecture: x86-64\n", COLOR_PASTEL_MINT);
    vga_print("  Kernel Version: 1.0.0\n", COLOR_PASTEL_LAVENDER);
    vga_print("  GUI System: Enabled\n", COLOR_PASTEL_BLUSH);
    
    vga_print("\n");
    vga_print("Theme: PASTEL (default)\n", COLOR_PASTEL_AMBER);
    vga_print("       [Press any key to switch to CYBER theme]\n", COLOR_PASTEL_CORAL);
    
    vga_print("\n=========================================\n", 
              (uint32_t)COLOR_PASTEL_WHITE.b | ((uint32_t)COLOR_PASTEL_BLUE.b << 8));
}

// ============================================================================
// Печать экрана о завершении загрузки
// ================================================================================

void print_completion_screen(void) {
    if (!gui_initialized) return;
    
    vga_clear((uint32_t)COLOR_CYBER_BLACK.b | ((uint32_t)COLOR_CYBER_GREEN.b << 8));
    
    vga_print("\n=========================================\n", 
              (uint32_t)COLOR_CYBER_BLACK.b | ((uint32_t)COLOR_CYBER_GREEN.b << 8));
    vga_print("         BOOT COMPLETE!                 \n", 
              (uint32_t)COLOR_CYBER_BLACK.b | ((uint32_t)COLOR_CYBER_GREEN.b << 8));
    vga_print("=========================================\n", 
              (uint32_t)COLOR_CYBER_BLACK.b | ((uint32_t)COLOR_CYBER_GREEN.b << 8));
    
    vga_print("\nSystem is ready for use.\n\n", COLOR_CYBER_PINK);
}
