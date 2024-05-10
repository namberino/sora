; 32-bit mode allows usage of 32-bit registers, memory addressing, protected memory, virtual memory, etc
; 32-bit mode will lose BIOS interrupts though (we'll need to code the GDT)
; This is a 32-bit print string routine without BIOS interrupts by directly manipulating VGA video memory instead of calling 'int 0x10'
; VGA memory starts at address 0xb8000 and it has text mode to avoid manipulating direct pixels
; Accessing specific character on an 80x25 grid (because every characters use 2 bytes, 1 for ascii, 1 for color): 0xb8000 + 2 * (row * 80 + col)
; Cannot yet call the subroutine from the bootloader because we don't have GDT and enter protected mode
; https://www.eecg.utoronto.ca/~amza/www.mindsec.com/files/x86regs.html
[bits 32] ; using 32-bit protected mode

; constants
VIDEO_MEMORY equ 0xb8000
WHITE_COLOR equ 0x0f ; color byte for each character

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY

print_string_pm_loop:
	mov al, [ebx] ; address of the character
	mov ah, WHITE_COLOR

	cmp al, 0 ; check if end of string
	je print_string_pm_done

	mov [edx], ax ; store character + attribute in video memory
	add ebx, 1 ; move to next character
	add edx, 2 ; move to next video memory location

	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret
