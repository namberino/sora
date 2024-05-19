#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct {
    uint32_t ds; // data segment register (for selecting)
    uint32_t edi, esi, ebp, useless, ebx, edx, ecx, eax; // pushed by pusha
    uint32_t int_no, err_code; // interrupt number and error code (if applicable)
    uint32_t eip, cs, eflags, esp, ss; // pushed by the processor automatically
} registers_t;

#endif
