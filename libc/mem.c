#include "mem.h"

// this should be computed at linking, but hardcoding this is fine for now, the kernel starts at 0x1000
uint32_t free_mem_address = 0x10000;

// set memory at specified location
void mem_set(uint8_t* destination, uint8_t value, uint32_t length)
{
    uint8_t* temp = (uint8_t*)destination;
    
    while (length != 0)
    {
        *temp++ = value;
        length--;
    }
    
}

// copy nbytes of value from 1 location to another location
void mem_copy(uint8_t* source, uint8_t* destination, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(destination + i) = *(source + i);
    }
}

// internal kmalloc
// can be used to request an aligned page, it will also return the real, physical address, for later use
uint32_t kmalloc_int(size_t size, int align, uint32_t* physical_address)
{
    // pages are aligned to 4kb or 0x1000
    if (align == 1 && (free_mem_address & 0xFFFFF000)) // checks if the address is not already aligned, if not then alight it
    {
        free_mem_address &= 0xFFFFF000; // get the 5 MSB (4kb)
        free_mem_address += 0x1000;
    }

    if (physical_address)
        *physical_address = free_mem_address; // point physical address to the 4kb free memory address

    uint32_t loc = free_mem_address; // location
    free_mem_address += size; // increment free address pointer to outside of specified size

    return loc;
}
