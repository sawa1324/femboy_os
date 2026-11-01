#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "framebuffer.h"
#include "console.h"

#define INPUT_SIZE 256

__attribute__((used, section(".limine_requests")))
static volatile uint64_t base_revision[3] = LIMINE_BASE_REVISION(6);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;

static void halt(void) {
    for (;;) {
        __asm__ volatile ("cli; hlt");
    }
}

void kmain(void) {
    if (!LIMINE_BASE_REVISION_SUPPORTED(base_revision))
        halt();

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1)
        halt();

    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    if (!framebuffer_init(fb))
        halt();

    console_init();
    timer_init();
    keyboard_init();

    console_write("FEMBOY OS\n");

    console_write("KERNEL INITIALIZED\n");
    console_write("FRAMEBUFFER READY\n\n");

    shell_init();

    char input[INPUT_SIZE];
    size_t length = 0;

    console_write("> ");

    for (;;) {
        console_update_cursor();

        int c = keyboard_getchar();

        if (c < 0) {
            __asm__ volatile ("pause");
            continue;
        }

        if (c == '\n') {
            input[length] = '\0';
            console_putchar('\n');

            shell_handle_line(input);

            length = 0;
            console_write("> ");
        }
        else if (c == '\b') {
            if (length > 0) {
                length--;
                console_putchar('\b');
            }
        }
        else if (c >= 32 && c <= 126) {
            if (length < INPUT_SIZE - 1) {
                input[length++] = (char)c;
                console_putchar((char)c);
            }
        }
    }
}
