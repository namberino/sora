Our kernel will just print an 'S' on the top left corner of the screen

# Steps

- Write and compile kernel code
- Write and assemble bootsector code
- Create kernel image that includes both the bootsector and the kernel
- Load kernel code into memory
- Switch to 32-bit protected mode
- Begin executing kernel code

# Compiling

```sh
i386-elf-gcc -ffreestanding -c kernel.c -o kernel.o
i386-elf-ld -o kernel.bin -Ttext 0x1000 kernel.o --oformat binary
```

> `0x1000` is the origin of this program

# Bootstrapping the kernel

- We need a bootsector that can bootstrap (load and begin executing) the kernel from disk
- Since kernel is compiled to 32-bit instructions, we need the bootsector to be in 32-bit protected mode
- BIOS will only load the bootsector and not the kernel. Since we're in 32-bit protected mode, we won't be able to use BIOS disk routines so we'll have to write a floppy or hard disk driver

- To simplify the problem of which disk and from which sectors to load the kernel, the bootsector and the kernel can be grafted into a *kernel image*, which can be written into the initial sectors of the boot disk, such that the bootsector code will always be the head of the kernel image

- Create kernel image with the following file concatenation command:
```sh
cat bootsect.bin kernel.bin > kernel-image
```

- We need to load the kernel code into a kernel offset address
- We load it in 16-bit real mode, then access it in 32-bit protected mode

- We load 15 segments (excluding boot segment) because it doesn't hurt to read additional sector, but it may hurt when trying to detect that we didn’t read enough sectors at this stage when we later add to
- This would increase the memory footprint of the kernel code. The computer would hang without warning, perhaps halfway though a routine that was split across an unloaded sector boundary (very bad)

# Find the way into the kernel

- When we boot the kernel, recklessly we jumped to, and therefore began execution from, the first instruction of the kernel code
- Thee C compiler can decide to place code and data whereever it chooses in the output file
- Since this kernel had a single function, and based on our previous obsrevations of how the compiler generates machine code, we might assume that the first machine code instruction is the first instruction of kernel’s entry function: `main`
- 
