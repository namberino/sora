# Sora: A mini operating system

Sora is a small operating system with a small bootloader and a small kernel

This is a learning project for me so the code is somewhat messy. I've documented everything quite throughly through so if you want to, you can go through the code and learn more about operating system development

![qemu emulation](img/os-emulate-qemu.png)

# Requirements

Before you can compile this, you need to install some tools and libraries first:

- [NASM](https://www.nasm.org/)
- [QEMU](https://www.qemu.org/)
- [i386-elf-toolchain](https://github.com/nativeos/i386-elf-toolchain/releases)
- [binutils](https://www.gnu.org/software/binutils/)
- [gmp](https://gmplib.org/)
- [mpfr](https://www.mpfr.org/)

# Project structure

- The [bootsector](bootsector) directory is some mini bootsector projects that I made to get used to programming the bootsector
- The [kernel](kernel) directory is where you'll find the source code for Sora. Compile and run the kernel there.

# Useful commands

| Commands | Description |
| --- | --- |
| `make` | Compile |
| `make run` | Compile and run|
| `make debug` | Run and debug in GDB |
| `make clean` | Remove object and binary files |

# Milestones

- [x] Simple bootloader in 16-bit real mode
- [x] Accessing the disk
- [x] Program the GDT
- [x] Switch to 32-bit protected mode
- [x] Simple kernel
- [x] Video driver
- [x] Interrupt Descriptor Table
- [x] Parse user input
- [x] Memory allocator
