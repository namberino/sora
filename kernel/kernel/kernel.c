#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main()
{
    isr_install();
    asm volatile("sti"); // set interrupt flag (call interrupt)
    //init_timer(50);

    init_keyboard();

    // testing some of the interrupts
    // __asm__ __volatile__("int $2");
    // __asm__ __volatile__("int $3");
}
