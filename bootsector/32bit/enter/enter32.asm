[bits 16]
switch_to_pm: ; switch to protected mode
    cli ; disable interrupts
    lgdt [gdt_descriptor] ; load the GDT descriptor
    
    mov eax, cr0 
    or eax, 0x1 ; set 32-bit mode bit in cr0
    mov cr0, eax

    jmp CODE_SEG:init_pm ; far jump by using a different segment (our 32-bit code)

[bits 32]
init_pm: ; initialize segment registers and the stack
    mov ax, DATA_SEG ; update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000 ; update the stack so it is right at the top of the free space
    mov esp, ebp

    call BEGIN_PM ; call begin protected mode section
