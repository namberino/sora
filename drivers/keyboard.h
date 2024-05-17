#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include <stdint.h>

#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT_DOWN 0x2A
#define LSHIFT_UP 0xAA
#define RSHIFT_DOWN 0x36
#define RSHIFT_UP 0xB6
#define CAPSLOCK_DOWN 0x3A
#define CAPSLOCK_UP 0xBA
#define SCANCODE_MAX 57

void init_keyboard();

#endif
