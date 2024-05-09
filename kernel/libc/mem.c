#include "mem.h"

// this should be computed at linking, but hardcoding this is fine for now, the kernel starts at 0x1000
u32 free_mem_address = 0x10000;

// set memory at specified location
void mem_set(u8* destination, u8 value, u32 length)
{
    u8* temp = (u8*)destination;
    
    while (length != 0)
    {
        *temp++ = value;
        length--;
    }
    
}

// copy nbytes of value from 1 location to another location
void mem_copy(u8* source, u8* destination, int nbytes)
{
    for (int i = 0; i < nbytes; i++)
    {
        *(destination + i) = *(source + i);
    }
}

// can be used to request an aligned page, it will also return the real, physical address, for later use
u32 kmalloc(u32 size, int align, u32* physical_address)
{
    // pages are aligned to 4kb or 0x1000
    if (align == 1 && (free_mem_address & 0xFFFFF000))
    {
        free_mem_address &= 0xFFFFF000;
        free_mem_address += 0x1000;
    }

    if (physical_address)
        *physical_address = free_mem_address;

    u32 loc = free_mem_address; // location
    free_mem_address += size; // increment free address pointer to outside of specified size

    return loc;
}
