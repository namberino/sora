#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"

void main()
{
    isr_install();
    irq_install();

    kprint("Keyboard input is enabled\nType END to halt the processor\n> ");

    // testing some of the interrupts
    // __asm__ __volatile__("int $2");
    // __asm__ __volatile__("int $3");
}

// handling user input
void user_input(char* input)
{
    if (strcmp(input, "END") == 0)
    {
        kprint("Stopping the CPU\n");
        asm volatile("hlt");
    }
    
    kprint("Input is: ");
    kprint(input);
    kprint("\n> ");
}
