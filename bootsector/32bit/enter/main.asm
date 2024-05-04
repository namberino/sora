; steps to enter 32-bit PM
; - Disable interrupts
; - Set a bit on the CPU control register cr0
; - Flush the CPU pipeline by issuing a carefully crafted far jump
; - Update all the segment registers
; - Update the stack
; - Call to a well-known label which contains the first useful code in 32 bits
[org 0x7c00]
mov bp, 0x9000 ; set the stack
mov sp, bp

mov bx, MSG_REAL_MODE
call print ; This will be written after the BIOS messages

call switch_to_pm ; we never return from here

jmp $ ; this will actually never be executed

%include "../../functions/print.asm"
%include "../gdt/gdt.asm"
%include "../print/print.asm"
%include "enter32.asm"

[bits 32]
BEGIN_PM: ; after the switch we will get here
    mov ebx, MSG_PROT_MODE
    call print_string_pm ; this will be written at the top left corner

    jmp $

MSG_REAL_MODE db "Started in 16-bit real mode", 0
MSG_PROT_MODE db "Loaded 32-bit protected mode", 0

; bootsector magic numbers
times 510-($-$$) db 0
dw 0xaa55
