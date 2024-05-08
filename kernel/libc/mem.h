#ifndef MEM_H
#define MEM_H

#include "../cpu/types.h"

void mem_set(u8* destination, u8 value, u32 length);
void mem_copy(u8* source, u8* destination, int nbytes);

#endif
