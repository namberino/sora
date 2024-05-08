#ifndef IDT_H
#define IDT_H

#include "types.h"

// segment to select
#define KERNEL_CS 0x08

// how interrupt gate (handler) is defined 
typedef struct {
    u16 low_offset; // lower 16 bits of the address to jump to when this interrupt fires (handler address)
    u16 sel; // kernel segment selector
    u8 always0; // this needs to be always 0
    u8 flags; // flags
    u16 high_offset; // higher 16 bits of the address to jump to when this interrupt fires (handler address)
    
    /* 
    First byte:
    - Bit 7: "Interrupt is present"
    - Bits 6-5: Privilege level of caller (0 = kernel - 3 = user)
    - Bit 4: Set to 0 for interrupt gates
    - Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" 
    */
} __attribute__((packed)) idt_gate_t;

// pointer to the array of interrupt handlers
// assembly instruction 'lidt' will read it 
typedef struct {
    u16 limit;
    u32 base; // address of the first element in idt entry 
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
extern idt_gate_t idt[IDT_ENTRIES]; // how idt entries are stored
extern idt_register_t idt_reg; // the idt structure the cpu will load: a memory address and a size (similar to GDT register)

void set_idt_gate(int n, u32 handler);
void set_idt();

#endif
