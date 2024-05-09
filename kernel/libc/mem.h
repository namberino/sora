#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void mem_set(uint8_t* destination, uint8_t value, uint32_t length);
void mem_copy(uint8_t* source, uint8_t* destination, int nbytes);
uint32_t kmalloc(size_t size, int align, uint32_t* physical_address); // currently there's no kfree() yet

#endif
