#include "hlt.h"

// clear interrupts then halt program execution
void halt()
{
    asm volatile("cli");
    asm volatile("hlt");
}
