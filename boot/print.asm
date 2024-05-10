print:
	pusha

; check each characters for end of string character
start:
	mov al, [bx] ; base address for string
	cmp al, 0
	je done

	mov ah, 0x0e
	int 0x10 

	add bx, 1
	jmp start

done:
	popa
	ret

print_newline:
	pusha

	mov ah, 0x0e
	mov al, 0x0a ; newline character
	int 0x10
	mov al, 0x0d ; cariage return
	int 0x10

	popa
	ret

