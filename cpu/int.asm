[extern isr_handler] ; found in isr.c
[extern irq_handler] ; found in isr.c

isr_common_stub: ; saves the processor state, sets up for kernel mode segments, calls the C-level fault handler, and finally restores the stack frame
    pusha ; pushes edi, esi, ebp, esp, ebx, edx, ecx, eax
    mov ax, ds ; lower 16-bits of eax = ds
    push eax ; save data segment descriptor/selector
    mov ax, 0x10 ; load kernel data segment descriptor
    mov ds, ax ; set segment register to kernel data segment
    mov es, ax ; set segment register to kernel data segment
    mov fs, ax ; set segment register to kernel data segment
    mov gs, ax ; set segment register to kernel data segment
    push esp ; register* reg

    ; call C-level fault handler
    cld ; C code following the sysV ABI requires DF to be clear on function entry
    call isr_handler

    ; restore stack frame (previous processor state)
    pop eax ; clear pointer
    pop eax ; restore previous data segment descriptor
    mov ds, ax ; restore segment register to previous state 
    mov es, ax ; restore segment register to previous state 
    mov fs, ax ; restore segment register to previous state 
    mov gs, ax ; restore segment register to previous state
    popa
    add esp, 8 ; cleans up the pushed error code and pushed isr number
    iret ; pop 5 things at once: cs, eip, eflags, ss, and esp

; similar to the isr code except for the irq_handler call and the pop ebx
irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp

    cld
    call irq_handler

    pop ebx ; clear pointer
    pop ebx ; reload original data segment descriptor
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    iret

; we don't know which interrupt was caller when the handler is run, so we will need to have a different handler for every interrupt
; some interrupts push an error code onto the stack but others don't
; we push a dummy error code for the ones that don't, so that we have a consistent stack for all of them.

; global isr (so other code can access it)
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

; Divide By Zero exception
isr0:
    cli
    push byte 0
    push byte 0
    jmp isr_common_stub

; Debug exception
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

; Non Maskable Interrupt exception
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

; Breakpoint exception
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

; Into detected overflow exception
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

; Out of Bounds exception
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

; Invalid Opcode exception
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

; Coprocessor Not Available exception
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

; Double Fault exception (Has error code)
isr8:
    cli
    push byte 8
    jmp isr_common_stub

; Coprocessor Segment Overrun exception
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

; Bad TSS exception (Has error code)
isr10:
    cli
    push byte 10
    jmp isr_common_stub

; Segment Not Present exception (Has error code)
isr11:
    cli
    push byte 11
    jmp isr_common_stub

; Stack Fault exception (Has error code)
isr12:
    cli
    push byte 12
    jmp isr_common_stub

; General Protection Fault exception (Has error code)
isr13:
    cli
    push byte 13
    jmp isr_common_stub

; Page Fault exception (Has error code)
isr14:
    cli
    push byte 14
    jmp isr_common_stub

; Reserved exception
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

; Floating Point exception
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

; Alignment Check exception
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

; Machine Check exception
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; Reserved
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

; Reserved
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

; Reserved
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

; Reserved
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

; Reserved
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

; Reserved
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

; Reserved
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

; Reserved
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

; Reserved
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

; Reserved
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

; Reserved
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

; Reserved
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

; Reserved
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

; irq handlers
irq0:
	cli
	push byte 0
	push byte 32
	jmp irq_common_stub

irq1:
	cli
	push byte 1
	push byte 33
	jmp irq_common_stub

irq2:
	cli
	push byte 2
	push byte 34
	jmp irq_common_stub

irq3:
	cli
	push byte 3
	push byte 35
	jmp irq_common_stub

irq4:
	cli
	push byte 4
	push byte 36
	jmp irq_common_stub

irq5:
	cli
	push byte 5
	push byte 37
	jmp irq_common_stub

irq6:
	cli
	push byte 6
	push byte 38
	jmp irq_common_stub

irq7:
	cli
	push byte 7
	push byte 39
	jmp irq_common_stub

irq8:
	cli
	push byte 8
	push byte 40
	jmp irq_common_stub

irq9:
	cli
	push byte 9
	push byte 41
	jmp irq_common_stub

irq10:
	cli
	push byte 10
	push byte 42
	jmp irq_common_stub

irq11:
	cli
	push byte 11
	push byte 43
	jmp irq_common_stub

irq12:
	cli
	push byte 12
	push byte 44
	jmp irq_common_stub

irq13:
	cli
	push byte 13
	push byte 45
	jmp irq_common_stub

irq14:
	cli
	push byte 14
	push byte 46
	jmp irq_common_stub

irq15:
	cli
	push byte 15
	push byte 47
	jmp irq_common_stub
