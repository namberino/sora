# Compiling

- To compile system-independent code, we need the flag ``-ffreestanding`, so compile `function.c` in this fashion:
```sh
i386-elf-gcc -ffreestanding -c function.c -o function.o
```

- Examine how the machine code was put together:
```sh
i386-elf-objdump -d function.o
```

- What is the offset for where our function will be placed in memory? We don't actually know
- We'll place the offset at 0x0 and use `binary` format which generates machine code without any labels and/or metadata
- Metadata can be useful for debugging
- Since we're writing an operating system, metadata is no good. An unaware CPU will just execute every byte as machine code
- `Ttext 0x0` works in the same ways as `org`, it allows us to tell the compiler to offset label addresses in our code (eg: for any data we specify in the code, such as strings like 'Hello, World') to their absolute memory addresses when later loaded to a specific origin in memory
- When we load the kernel code into memory, it's important we set this to the address we plan to load to

- Linking the object to make a binary:
```sh
i386-elf-ld -o function.bin -Ttext 0x0 --oformat binary function.o
```

> Run `xxd` on the binary and object files will show us that the object file still have the debugging information and the binary file is machine code

# Decompiling

- `-b 32` tells the disassembler to decode to 32-bit assembly instructions
- Decompiling the program:
```sh
ndisasm -b 32 function.bin
```
