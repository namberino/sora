#include "ports.h"

// weird assembly syntax stuff

// read bytes from specific port
uint8_t port_byte_in(uint16_t port)
{
    /*
    Inline assembly syntax:
    source and destination registers are switched from nasm
    '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
    '"d" (port)': map the C variable '(port)' into e'd'x register
    */
    uint8_t result;
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    // nothing is returned so no '='
    // since there are two variables in the input area and none in the 'return' area so 2 commas there
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in(unsigned short port)
{
    uint16_t result;
    asm("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data)
{
    asm volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint32_t port_dword_in(uint16_t port)
{
    uint32_t result;
    asm("in %%dx, %%eax" : "=a" (result) : "d" (port)); 
    return result;
}

void port_dword_out(uint16_t port, uint16_t data)
{
    asm volatile("out %%eax, %%dx" : : "a" (data), "d" (port));
}
