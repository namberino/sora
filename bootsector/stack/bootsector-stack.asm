mov ah, 0x0e

mov bp, 0x8000 ; far location from 0x7c00
mov sp, bp ; initialize stack pointer (empty)

push 'A'
push 'B'
push 'C'

; stack grows downward
mov al, [0x7ffe] ; 0x8000 - 2
int 0x10

; this won't work since you can only access the stack from the top
mov al, [0x8000]
int 0x10

; recover character using pop
; can only pop full words so we need an auxiliary register to manipulate the lower byte
pop bx
mov al, bl
int 0x10 ; prints C

pop bx
mov al, bl
int 0x10 ; prints B

pop bx
mov al, bl
int 0x10 ; prints A

; now the stack contains garbage values
mov al, [0x8000]
int 0x10

jmp $

times 510-($-$$) db 0
dw 0xaa55
