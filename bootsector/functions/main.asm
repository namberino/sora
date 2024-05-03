[org 0x7c00]

mov bx, HELLO
call print
call print_newline

mov bx, GOODBYE
call print
call print_newline

mov dx, 0x3456
call print_hex

jmp $

%include "print.asm"
%include "print-hex.asm"

HELLO:
	dw 'Hello world', 0

GOODBYE:
	dw 'Goodbye', 0

times 510-($-$$) db 0
dw 0xaa55
