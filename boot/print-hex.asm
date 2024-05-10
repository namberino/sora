; receive data in dx (16-bit)
print_hex:
	pusha

	mov cx, 0 ; index variable

; get last char of dx and turn to ascii
; Conversion: Add 0x30 to number byte to turn it to ascii. Add 0x40 to char byte to turn it into ascii
; then move the ascii byte to the correct position on the resulting string
hex_loop:
	cmp cx, 4 ; loop 4 times
	je exit
	
	; convert last char of dx to ascii
	mov ax, dx 
	and ax, 0x000f ; get last byte of dx
	add al, 0x30 ; convert byte to ascii
	cmp al, 0x39 ; if byte > 9, it's A-F
	jle step2
	add al, 7 ; since 'A' is ascii 65 instead of 58, add 7 to make it 65

; get correct position of string to place the ascii char
; bx = base address + string length - index of char
step2:
	mov bx, HEX_OUT + 5 ; base + length
	sub bx, cx ; index variable
	mov [bx], al ; copy char on al to position pointed to by bx
	ror dx, 4 ; move to next byte of dx

	add cx, 1 ; increment
	jmp hex_loop ; return to loop

exit:
	mov bx, HEX_OUT ; print receive parameter in bx
	call print

	popa
	ret
	
HEX_OUT:
	db '0x0000',0 ; reserve memory for our new string
