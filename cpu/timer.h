#ifndef TIMER_H
#define TIMER_H

#include "isr.h"
#include "ports.h"
#include "../libc/function.h"
#include <stdint.h>

void init_timer(uint32_t frequency);
void set_pit2(uint32_t frequency);
void halt_timer(uint32_t halt_ticks);

#endif
