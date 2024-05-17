#include "isr.h"

isr_t int_handlers[256];

// can't do this in a loop cuz we need the address of the function names
void isr_install()
{
    // default processor interrupts
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    // remap the PIC (reference https://wiki.osdev.org/PIC)
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0); 

    // install the irq
    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt(); // set idt using lidt
}

char* exception_msg[] = {
    "Division by Zero",
    "Debug exception",
    "Non maskable interrupt",
    "Breakpoint exception",
    "Into detected overflow",
    "Out of bounds exception",
    "Invalid opcode exception",
    "No coprocessor exception",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt exception",
    "Coprocessor fault",
    "Alignment check exception",
    "Machine check exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

// we print the interrupt code and interrupt message
void isr_handler(registers_t* reg)
{
    kprint("Interrupt: ");
    char str[3];
    int_to_ascii(reg->int_no, str);
    kprint(str);
    kprint("\n");
    kprint(exception_msg[reg->int_no]);
    kprint("\n");
}

// register the interrupt handler
void register_int_handler(uint8_t n, isr_t handler)
{
    int_handlers[n] = handler;
}

// gets called from asm interrupt handler stub
void irq_handler(registers_t* reg)
{
    // send an EOI (end of interrupt) signal to the PICs after every interrupts (if this interrupt involved the slave)
    if (reg->int_no >= 40)
        port_byte_out(0xA0, 0x20); // send reset signal to slave

    port_byte_out(0x20, 0x20); // send reset signal to master

    // interrupt handling
    if (int_handlers[reg->int_no] != 0)
    {
        isr_t handler = int_handlers[reg->int_no];
        handler(reg);
    }
}

// install the timer and keyboard irq
void irq_install()
{
    asm volatile("sti"); // enable interrupts
    init_timer(50); // initialize IRQ0: timer (50Hz)
    init_keyboard(); // initialize IRQ1: keyboard
}
