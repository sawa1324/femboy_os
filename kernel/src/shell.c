#include <stddef.h>
#include <stdint.h>
#include "shell.h"
#include "console.h"
#include "timer.h"
#include "framebuffer.h"
#include "assets/logo.h"

static int string_equal(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b)
            return 0;
        a++;
        b++;
    }
    return *a == *b;
}

static int starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str != *prefix)
            return 0;
        str++;
        prefix++;
    }
    return 1;
}

static void print_number(uint64_t value) {
    char buffer[21];
    int i = 0;

    if (value == 0) {
        console_putchar('0');
        return;
    }

    while (value > 0) {
        buffer[i++] = '0' + (value % 10);
        value /= 10;
    }

    while (i > 0)
        console_putchar(buffer[--i]);
}

static void shell_neofetch(void) {

    console_write("Resolution: ");
    print_number(framebuffer_width());
    console_write("x");
    print_number(framebuffer_height());
    console_write("\\n");

    framebuffer_draw_grayscale(
        24, 32, 192, 126,
        logo_pixels, LOGO_WIDTH, LOGO_HEIGHT
    );

    console_set_cursor(32, 3);

    console_write("FEMBOY OS\n");
    console_write("-------------------------\n");
    console_write("OS:       Femboy OS\n");
    console_write("Version:  0.2.0\n");
    console_write("Kernel:   Custom x86_64\n");
    console_write("Shell:    Femboy Shell 0.2\n");
    console_write("User:     root\n");
    console_write("Uptime:   ");

    print_number(timer_milliseconds() / 1000);
    console_write(" seconds\n");

    console_write("Theme:    ");
    if (console_get_theme() == CONSOLE_THEME_PASTEL)
        console_write("Pastel\n");
    else
        console_write("Dark\n");

    console_write("\n");
}

void shell_init(void) {
    shell_neofetch();
}

void shell_handle_line(const char *line) {
    if (line == NULL || line[0] == '\0')
        return;

    if (string_equal(line, "help")) {
        console_write("Available commands:\n");
        console_write("  help     - Show commands\n");
        console_write("  about    - About Femboy OS\n");
        console_write("  version  - Show system version\n");
        console_write("  clear    - Clear screen\n");
        console_write("  echo     - Print text\n");
        console_write("  whoami   - Show current user\n");
        console_write("  uname    - Show system information\n");
        console_write("  uptime   - Show system uptime\n");
        console_write("  theme    - Switch color theme\n");
        console_write("  neofetch - Show system information\n");
    }
    else if (string_equal(line, "about")) {
        console_write("Femboy OS is an independent x86_64 OS.\n");
    }
    else if (string_equal(line, "version")) {
        console_write("Femboy OS 0.2.0\n");
    }
    else if (string_equal(line, "whoami")) {
        console_write("root\n");
    }
    else if (string_equal(line, "uname")) {
        console_write("Femboy OS x86_64\n");
    }
    else if (string_equal(line, "uptime")) {
        uint64_t seconds = timer_milliseconds() / 1000;
        console_write("Uptime: ");
        print_number(seconds);
        console_write(" seconds\n");
    }
    else if (string_equal(line, "theme")) {
        enum console_theme next =
            console_get_theme() == CONSOLE_THEME_DARK
                ? CONSOLE_THEME_PASTEL
                : CONSOLE_THEME_DARK;

        console_set_theme(next);

        if (next == CONSOLE_THEME_PASTEL)
            console_write("Theme changed to Pastel.\n");
        else
            console_write("Theme changed to Dark.\n");
    }
    else if (string_equal(line, "theme dark")) {
        console_set_theme(CONSOLE_THEME_DARK);
        console_write("Theme changed to Dark.\n");
    }
    else if (string_equal(line, "theme pastel")) {
        console_set_theme(CONSOLE_THEME_PASTEL);
        console_write("Theme changed to Pastel.\n");
    }
    else if (string_equal(line, "neofetch")) {
        console_clear();
        shell_neofetch();
    }
    else if (string_equal(line, "clear")) {
        console_clear();
    }
    else if (starts_with(line, "echo ")) {
        console_write(line + 5);
        console_putchar('\n');
    }
    else {
        console_write("Unknown command: ");
        console_write(line);
        console_putchar('\n');
    }
}
