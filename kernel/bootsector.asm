; boots the C kernel into 32-bit protected mode
[org 0x7c00]
KERNEL_OFFSET equ 0x1000 ; this is the memory offset to which the kernel will be loaded

mov [BOOT_DRIVE], dl ; BIOS stores the boot drive in dl

mov bp, 0x9000 ; setup stack
mov sp, bp

mov bx, MSG_REAL_MODE ; print booting into 16-bit real mode
call print
call print_newline

call load_kernel ; load the kernel

call switch_to_pm ; switch to protected mode, we won't return from this

jmp $

%include "../bootsector/functions/print.asm"
%include "../bootsector/functions/print-hex.asm"
%include "../bootsector/disk/disk.asm"
%include "../bootsector/32bit/gdt/gdt.asm"
%include "../bootsector/32bit/print/print.asm"
%include "../bootsector/32bit/enter/enter32.asm"

[bits 16]
load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_newline

    ; setup parameters for disk_load routine
    ; load the first 15 sectors (excluding the bootsector) from the boot disk (the kernel code) to address KERNEL_OFFSET
    mov bx, KERNEL_OFFSET 
    mov dh, 15 
    mov dl, [BOOT_DRIVE]
    call disk_load

    ret

; this is where we arrive after switching to and initializing protected mode
[bits 32]
BEGIN_PM:
    mov ebx, MSG_PROTECT_MODE ; use 32-bit print routine to announce
    call print_string_pm

    call KERNEL_OFFSET ; now jump to the address of the loaded kernel code 

    jmp $ ; hang

BOOT_DRIVE db 0 ; It is a good idea to store it in memory because 'dl' may get overwritten
MSG_REAL_MODE db "Started in 16-bit Real Mode", 0
MSG_PROTECT_MODE db "Landed in 32-bit Protected Mode", 0
MSG_LOAD_KERNEL db "Loading kernel into memory", 0

times 510-($-$$) db 0
dw 0xaa55
