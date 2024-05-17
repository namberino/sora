C_SRC = $(wildcard kernel/*.c drivers/*.c cpu/*.c libc/*.c)
HEADERS = $(wildcard kernel/*.h drivers/*.h cpu/*.h libc/*.h)
ASM_SRC = $(wildcard cpu/*.asm)
OBJ = ${C_SRC:.c=.o} ${ASM_SRC:.asm=.o} # file extension replacement

CC = /opt/local/bin/i386-elf-gcc # change this to path to your i386 gcc
GDB = /usr/local/bin/i386-elf-gdb # change this to path to your i386 gdb
CFLAGS = -g -m32 -ffreestanding -fno-PIC -fno-stack-protector -nostartfiles -nodefaultlibs -Wall -Wextra -Werror

# run by default
sora.img: boot/bootsector.bin kernel.bin
	cat $^ > sora.bin
	dd if=/dev/zero bs=512 count=2880 >> $@
	dd if=sora.bin of=$@ conv=notrunc

# '--oformat binary' automatically strips symbols
kernel.bin: boot/kernel-entry.o ${OBJ}
	i386-elf-ld -no-pie -o $@ -Ttext 0x1000 $^ --oformat binary --allow-multiple-definition

# for debugging
kernel.elf: boot/kernel-entry.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^

run: sora.img
	qemu-system-i386 -fda $< -audiodev alsa,id=default -machine pcspk-audiodev=default

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
	rm -rf *.bin *.dis *.o *.elf sora.img
	rm -rf kernel/*.o boot/*.bin drivers/*.o boot/*.o cpu/*.o libc/*.o
