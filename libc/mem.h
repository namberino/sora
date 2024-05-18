#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void mem_set(uint8_t* destination, uint8_t value, uint32_t length);
void mem_copy(uint8_t* source, uint8_t* destination, int nbytes);
uint32_t kmalloc_int(size_t size, int align, uint32_t* physical_address);
uint32_t kmalloc_a(u32int sz); // page must be aligned
uint32_t kmalloc_p(u32int sz, u32int *physical_address); // returns physical address to physical_address
uint32_t kmalloc_ap(u32int sz, u32int *physical_address); // page aligned and return to physical_address
uint32_t kmalloc(u32int sz); 

#endif
