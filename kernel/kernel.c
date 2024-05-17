#include "kernel.h"

void kernel_main()
{
    clear_screen();
    isr_install();
    irq_install();

    // testing some interrupts
    // asm volatile("int $2");
    // asm volatile("int $3");

    kprint_color("Hello World! I am Sora OS.\n", OUTPUT_COLOR);
    kprint_color("Type HELP for the list of available commands\n\n", OUTPUT_COLOR);
    kprint_cmd_prompt();
}

void kprint_help()
{
    kprint_color("BEEP: Play beep sound\n", OUTPUT_COLOR);
    kprint_color("CLEAR: Clear the screen\n", OUTPUT_COLOR);
    kprint_color("PAGE: Request a page allocation\n", OUTPUT_COLOR);
    kprint_color("END: Stop the CPU\n", OUTPUT_COLOR);
}

void kprint_cmd_prompt()
{
    kprint_color("> ", CMD_PROMPT_COLOR);
}

// handling user input
void user_input(char* input)
{
    if (strcmp(input, "END") == 0)
    {
        kprint_color("Stopping the CPU\n", OUTPUT_COLOR);
        asm volatile("hlt");
    }
    else if (strcmp(input, "HELP") == 0)
    {
        kprint_help();
    }
    else if (strcmp(input, "PAGE") == 0)
    {
        uint32_t physical_address;
        uint32_t page = kmalloc(1000, 1, &physical_address);

        char page_str[16] = "";
        hex_to_ascii(page, page_str);
        char physical_address_str[16] = "";
        hex_to_ascii(physical_address, physical_address_str);

        kprint_color("Page: ", OUTPUT_COLOR);
        kprint_color(page_str, VAR_COLOR);
        kprint_color(", physical address: ", OUTPUT_COLOR);
        kprint_color(physical_address_str, VAR_COLOR);
        kprint("\n");
    }
    else if (strcmp(input, "CLEAR") == 0)
    {
        clear_screen();
    }
    else if (strcmp(input, "BEEP") == 0)
    {
        beep();
    }
    else
    {
        kprint_color("Invalid command: ", ERROR_COLOR_1);
        kprint_color(input, CMD_COLOR);
        kprint("\n");
    }
    
    // kprint("Input is: ");
    // kprint(input);
    // kprint("\n");
    kprint_cmd_prompt();
}
