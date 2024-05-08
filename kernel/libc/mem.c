#include "mem.h"

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
