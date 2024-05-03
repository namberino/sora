[org 0x7c00]

mov bp, 0x8000 ; set stack safely away from bootsector
mov sp, bp

mov bx, 0x9000 ; es:bx = 0x0000:0x9000 = 0x09000
mov dh, 2 ; read 2 sectors
call disk_load

mov dx, [0x9000] ; retrieve first loaded word: 0xdada
call print_hex
call print_newline

mov dx, [0x9000 + 512] ; first word of sector 3: 0xface
call print_hex

jmp $

%include "../functions/print.asm"
%include "../functions/print-hex.asm"
%include "disk.asm"

times 510 - ($-$$) db 0
dw 0xaa55

; bootsector (512 bytes) = sector 1 of cylinder 0 of head 0 of drive 0
times 256 dw 0xdada ; sector 2 = 512 bytes
times 256 dw 0xface ; sector 3 = 512 bytes
