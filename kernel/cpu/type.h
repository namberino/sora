#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

// // 32-bit
// typedef unsigned int u32;
// typedef int s32;

// // 16-bit
// typedef unsigned short u16;
// typedef short s16;

// // 8-bit
// typedef unsigned char u8;
// typedef char s8;

#define low_16(address) (uint16_t)((address) & 0xFFFF) // get lower 16 bytes
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF) // get higher 16 bytes

#endif
