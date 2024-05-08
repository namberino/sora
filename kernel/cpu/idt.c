#include "idt.h"
#include "../kernel/util.h"

idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

// set struct values
void set_idt_gate(int n, u32 handler) // interrupt number and handler
{
    idt[n].low_offset = low_16(handler);
    idt[n].sel = KERNEL_CS;
    idt[n].always0 = 0;
    idt[n].flags = 0x8E; // see osdev reference in the interrupt.md
    idt[n].high_offset = high_16(handler);
}

// call lidt command
void set_idt()
{
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    // don't make the mistake of loading &idt, always load &idt_reg (because idt_reg is the pointer to the array of the interrupt handlers)
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}
