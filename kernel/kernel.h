#ifndef KERNEL_H
#define KERNEL_H

#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../drivers/speaker.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

void user_input(char* input);

#endif
