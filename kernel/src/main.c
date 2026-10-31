
#include <stdint.h>
#include <stddef.h>
#include <limine.h>
#include "framebuffer.h"
#include "console.h"

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
    if (!LIMINE_BASE_REVISION_SUPPORTED(base_revision)) {
        halt();
    }

    if (framebuffer_request.response == NULL ||
        framebuffer_request.response->framebuffer_count < 1) {
        halt();
    }

    struct limine_framebuffer *fb =
        framebuffer_request.response->framebuffers[0];

    if (!framebuffer_init(fb)) {
        halt();
    }

    console_init();

    console_set_color(0xD88BC7, 0x10121C);
    console_write("FEMBOY OS\n");

    console_set_color(0xE6E6F0, 0x10121C);
    console_write("INSTALLER 0.0.1\n");
    console_write("\n");
    console_write("KERNEL INITIALIZED\n");
    console_write("FRAMEBUFFER READY\n");
    console_write("\n");
    console_write("SYSTEM CONSOLE\n");
    console_write("> ");

    halt();
}

