; we need to load data from disk to run the kernel
; disk interrupt: set al to 0x02 and call int 0x13

; we'll need to use the carry bit
; the BIOS also sets al to the number of sectors read, so always compare it to the expected number

; int 0x13 usage: https://stanislavs.org/helppc/int_13-2.html
; error code: https://stanislavs.org/helppc/int_13-1.html

disk_load: ; load 'dh' sectors from drive 'dl' into ES:BX
	pusha
	; reading from disk requires setting specific values in all registers 
	; put dx data to stack for later use as we'll overwrite the input parameter of dx
	push dx

	mov ah, 0x02 ; read mode
	mov al, dh   ; number of sectors to read (0x01 - 0x80 or 1 - 128)
	mov cl, 0x02 ; sector number (0x01 - 0x11 or 1 - 17). 0x01 is the bootsector, 0x02 is the first available sector
	mov ch, 0x00 ; track/cylinder number (This is a 10-bit value: 0x00 - 0x3FF or 0 - 1023. with the 2 most significant bits of CL being the high order bits and 8 bits of CH being the low order bits)
	; dl is drive number. The caller sets it as a parameter and gets it from BIOS (0 = floppy, 1 = 2nd floppy, 0x80 = drive 0 - hdd, 0x80 = drive 1 - 2nd hdd)
	mov dh, 0x00 ; head number (0x0 - 0xF or 0 - 15)

	; [es:bx]: es is the 16-bit segment number and bx is the 16-bit offset. This give an address of "segment * 0x10 + offset", making a 20-bit address in real mode. 
	; [es:bx] is pointer to buffer (where data will be stored). The standard location for interrupt 0x13
	; Caller will set this up
	int 0x13
	jc disk_error ; carry bit will be set if there's an error

	pop dx
	cmp al, dh ; BIOS sets al to the number of sectors read, compare it to make sure we read the expected number of sectors
	jne sector_error
	popa
	ret

disk_error:
	mov bx, DISK_ERROR
	call print
	call print_newline
	mov dh, ah ; ah = error code, dl = disk drive that dropped the error
	call print_hex
	jmp disk_loop

sector_error:
	mov bx, SECTOR_ERROR
	call print

disk_loop:
	jmp $

DISK_ERROR: db "Disk read error", 0
SECTOR_ERROR: db "Incorrect number of sectors read", 0
