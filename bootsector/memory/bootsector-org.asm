; This sets the origin (starting point) of the program
[org 0x7c00]

mov ah, 0x0e

; dereference secret pointer
mov al, [secret]
int 0x10

jmp $

secret:
	db "S"

times 510-($-$$) db 0
dw 0xaa55
