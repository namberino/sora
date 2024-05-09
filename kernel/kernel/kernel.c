#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "kernel.h"
#include "../libc/string.h"
#include "../libc/mem.h"

void main()
{
    isr_install();
    irq_install();

    clear_screen();
    kprint("Hello World! I am Sora\n\nKeyboard input is enabled\nType PAGE to request a kmalloc\nType END to halt the processor\n> ");

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
    else if (strcmp(input, "PAGE") == 0)
    {
        u32 physical_address;
        u32 page = kmalloc(1000, 1, &physical_address);

        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char physical_address_str[16] = "";
        hex_to_ascii(physical_address, physical_address_str);

        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(physical_address_str);
        kprint("\n");
    }
    
    kprint("Input is: ");
    kprint(input);
    kprint("\n> ");
}
