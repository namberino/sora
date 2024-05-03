; bootsector is stored at 0x7c00 and takes up 512 bytes (to 0x7e00)

; This prints the memory address instead of the content stored in the memory address
mov ah, 0x0e 
mov al, '1'
int 0x10
mov al, secret
int 0x10

; This tries to dereference the address of secret without the offset, so doesn't work
mov al, '2'
int 0x10
mov al, [secret]
int 0x10

; Add bootsector starting offset to X and dereference the pointer 
mov al, '3'
int 0x10
mov bx, secret
mov bx, 0x7c00
mov al, bx
int 0x10

; shortcut, not effective since we don't want to be 
mov al, '4'
int 0x10
mov al, 0x7c2d
int 0x10

secret:
	db "S"

times 510-($-$$) db 0
dw 0xaa55
