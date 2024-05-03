; [org] is also segmentation
; Segmentation means you can specify an offset to all the data you're referring to
; Done using special registers: 'cs', 'ds', 'ss', 'es' (for code, data, stack, and extra - user defined)
; They are implicitly used by the CPU, so once you set some value for, say 'ds', then all of your memory access will be offseted by 'ds'
; https://wiki.osdev.org/Segmentation

; To compute the real address we don't just join 2 addresses, we overlap them: segment << 4 + address
; Example: If 'ds' is 0x4d and then [0x20] then that refers to 0x4d0 + 0x20 = 0x4f0
; We cannot 'mov' literals to those registers, we have to use general purpose registers
mov ah, 0x0e

mov al, [secret] ; doesn't work since bootloader starts at 0x7c00
int 0x10

mov bx, 0x7c0 ; this segment will be automatically << 4 
mov ds, bx ; from now on, all memory will be implicitly offseted by ds
mov al, [secret]
int 0x10

mov al, [es:secret] ; es is currently 0x000 so nothing happens
int 0x10

mov bx, 0x7c0
mov es, bx
mov al, [es:secret] ; now this works
int 0x10

jmp $

secret:
	db 'S'

times 510-($-$$) db 0
dw 0xaa55
