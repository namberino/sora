#include "../drivers/screen.h"

void main()
{
    clear_screen();
    kprint_at("S", 1, 6);
    kprint_at("This is a text spanning multiple lines", 75, 10);
    kprint_at("A line\nbreak", 0, 20);
    kprint("Another line\nbreak");
    kprint_at("We might be running out of space here", 45, 24);
}
