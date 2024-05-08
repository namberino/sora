#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"

void main()
{
    isr_install();
    asm volatile("sti");
    init_timer(50);

    // testing some of the interrupts
    // __asm__ __volatile__("int $2");
    // __asm__ __volatile__("int $3");
}
