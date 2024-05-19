#ifndef KERNEL_H
#define KERNEL_H

#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../cpu/paging.h"
#include "../drivers/screen_colors.h"
#include "../drivers/screen.h"
#include "../drivers/speaker.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

void user_input(char* input);
void kprint_help();
void kprint_cmd_prompt();

#endif
