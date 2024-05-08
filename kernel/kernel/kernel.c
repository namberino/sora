#include "../drivers/screen.h"
#include "util.h"
#include "../cpu/isr.h"
#include "../cpu/idt.h"

void main()
{
    // clear_screen();

    // // fill up the screen
    // // print out line number on the first column
    // for (int i = 0; i < 24; i++)
    // {
    //     char str[255];
    //     int_to_ascii(i, str);
    //     kprint_at(str, 0, i);
    // }

    // kprint_at("This should force the kernel to scroll. Row 0 will disappear", 60, 24);
    // kprint("\nThis text will force kernel to scroll and make row 1 disappear");

    isr_install();

    // testing some of the interrupts
    __asm__ __volatile__("int $2");
    __asm__ __volatile__("int $3");
}
