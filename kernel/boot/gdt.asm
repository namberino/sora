; In 32-bit mode, offset becomes an index for a segment descriptor (SD) in the GDT
; This descriptor defines the segment base address (32-bit), the segment size (20-bits) and some flags (like readonly, permissions, etc)
; The data structures are split

; Easiest way to program GDT is to define 2 segments, 1 for code, another for data. These can overlap (no memory protection) but it's good enough to boot (fix this later with a higher level language)
; First GDT entry must be 0x00 to make sure the programmer didn't make mistakes managing addresses

; CPU can't directly load GDT address, it requires a meta structure called "GDT descriptor" with a 16-bit size and a 32-bit address.
; This is loaded with the 'lgdt' command
gdt_start: ; labels are needed to compute sizes and jumps
    ; start with a null 8-byte
    dd 0x0 ; 4 byte
    dd 0x0 ; 4 byte

; GDT for code segment. base = 0x00000000, length = 0xfffff
; 1st flags: present = 1, privilege = 00, descriptor type = 1 -> 4'b1001
; type flags: code = 1, conforming = 0, readable = 1, accessed = 0 -> 4'b1010
; 2nd flags: granularity = 1, 32-bit default = 1, 64-bit segment = 0, AVL = 0 -> 4'b1100
gdt_code:
    dw 0xffff    ; segment length or limit (bits 0 - 15)
    dw 0x0       ; segment base (bits 0 - 15)
    db 0x0       ; segment base (bits 16 - 23)
    db 10011010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, segment length or limit (bits 16 - 19)
    db 0x0       ; base (bits 24 - 31)

; GDT for data segment. base and length identical to code segment, some flags are changed
; type flags: code = 0, expand down = 0, writable = 1, accessed = 0 -> 4'b0010
gdt_data:
    dw 0xffff    ; segment length or limit (bits 0 - 15)
    dw 0x0       ; segment base (bits 0 - 15)
    db 0x0       ; segment base (bits 16 - 23)
    db 10010010b ; 1st flags, type flags
    db 11001111b ; 2nd flags, segment length or limit (bits 16 - 19)
    db 0x0       ; base (bits 24 - 31)

gdt_end: ; this label is here at the end of the GDT so the assembler can calculate the size of the GDT for the GDT descriptor 

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; size (16 bit), always one less than its true size
    dd gdt_start ; address (32 bit)

; define some constants for the segment descriptor offsets (what the segment registers must contain when in protected mode)
; example: set ds = 0x10 in protected mode, the CPU knows that this is using the segment described at offset 0x10 (16 bytes) in GDT, which in our case is the data segment (0x0 is null, 0x08 is code, 0x10 is data)
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
