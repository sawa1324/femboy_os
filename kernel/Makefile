CC = gcc

CFLAGS = -std=gnu11 -O2 -Wall -Wextra \
         -ffreestanding -fno-builtin \
         -fno-stack-protector -fno-pic -fno-pie \
         -fno-asynchronous-unwind-tables \
         -m64 -march=x86-64 \
         -mno-mmx -mno-sse -mno-sse2 -mno-red-zone \
         -mcmodel=kernel \
         -Iinclude

LDFLAGS = -nostdlib -static -no-pie \
          -Wl,-z,max-page-size=0x1000 \
          -Wl,-T,linker/x86_64.lds \
          -Wl,--build-id=none
SOURCES = src/main.c src/framebuffer.c src/console.c src/font.c src/memory.c src/keyboard.c
TARGET = build/kernel

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) linker/x86_64.lds
	mkdir -p build
	$(CC) $(CFLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

clean:
	rm -rf build
