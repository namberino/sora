#include "../drivers/screen.h"

void main()
{
    clear_screen();
    kprint_at("This is the NamOS kernel", 0, 0);
    kprint_at("Testing the VGA driver", 0, 2);
    kprint_at("Printing\nmultiple\nlines\n\n", 0, 4);
    kprint("More\nline\nbreaks");
    kprint_at("Might be running out of space here", 45, 24);
}
