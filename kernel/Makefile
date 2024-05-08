C_SRC = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
ASM_SRC = $(wildcard cpu/*.asm)
OBJ = ${C_SRC:.c=.o} ${ASM_SRC:.asm=.o} # file extension replacement

CC = /opt/local/bin/i386-elf-gcc
GDB = /usr/local/bin/i386-elf-gdb
CFLAGS = -g -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror

# run by default
kernel-image.bin: boot/bootsector.bin kernel.bin
	cat $^ > kernel-image.bin

# '--oformat binary' automatically strips symbols
kernel.bin: boot/kernel-entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

# for debugging
kernel.elf: boot/kernel-entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: kernel-image.bin
	qemu-system-i386 -fda kernel-image.bin

# open connection to qemu on port 1234
# load kernel object with symbols
debug: kernel-image.bin kernel.elf
	qemu-system-i386 -s -fda kernel-image.bin -d guest_errors, int &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# rules for wildcards
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm -f elf $< -o $@

%.bin: %.asm
	nasm -f bin $< -o $@

clean:
	rm -rf *.bin *.dis *.o *.elf kernel-image.bin 
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
