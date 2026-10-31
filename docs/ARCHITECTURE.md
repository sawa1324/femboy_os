
# Femboy OS — Architecture

## 1. Vision

Femboy OS is an independent operating system for x86-64 computers.
It will have its own kernel and will not be based on the Linux kernel.

The initial goal is a small, bootable, testable system that can grow
into a complete desktop operating system.

## 2. Initial platform

- Architecture: x86-64
- Development host: Windows with Ubuntu on WSL
- Virtual machine: QEMU
- Bootloader: Limine
- Kernel languages: C and x86-64 Assembly
- Build system: Make

## 3. Kernel design

The kernel will initially use a modular monolithic architecture.

Planned components:

- Boot and early initialization
- Physical and virtual memory management
- Interrupt and exception handling
- CPU and timer management
- Basic device drivers
- Kernel logging and debugging

Features will be introduced incrementally and tested in QEMU.

## 4. Desktop roadmap

Long-term goals:

- A graphical desktop environment
- A file manager and terminal
- Networking and audio
- Input device support
- 2D graphics, followed by 3D acceleration
- Applications and games
- An independent update system

Linux application compatibility is a long-term goal, not an initial feature.

## 5. Development principles

- Keep each milestone small and testable.
- Prefer clear interfaces between kernel components.
- Test changes in a virtual machine before real hardware.
- Keep source code and build instructions in version control.
- Document design decisions and known limitations.
- Review third-party dependencies and their licenses.

## 6. First milestone

Boot a minimal x86-64 kernel in QEMU using Limine.
Confirm that the kernel starts and can write a message to the debug output.

