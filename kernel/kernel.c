#include "kernel.h"

void kernel_main()
{
    isr_install();
    irq_install();
    //clear_screen();

    // testing some interrupts
    asm volatile("int $2");
    asm volatile("int $3");

    kprint("Hello World! I am Sora\n\nKeyboard input is enabled\nType PAGE to request a kmalloc\nType END to halt the processor\n> ");
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
        uint32_t physical_address;
        uint32_t page = kmalloc(1000, 1, &physical_address);

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
