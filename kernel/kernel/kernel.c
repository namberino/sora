#include "../drivers/ports.h"

void main()
{
    // screen cursor position: ask VGA control register (0x3d4) for bytes. 14 = high byte of cursor and 15 = low byte of cursor
    port_byte_out(0x3d4, 14); // requesting byte 14: high byte of cursor pos

    // data is returned in vga data register (0x3d5)
    int pos = port_byte_in(0x3d5);
    pos = pos << 8; // shift to high byte 

    port_byte_out(0x3d4, 15); // requesting low byte
    pos += port_byte_in(0x3d5);

    // vga cells consist of the character and its control data (white text on black background, red text on white background, etc)
    // multiply by 2 to take into account these 2 bytes for each cells
    int offset_from_vga = pos * 3;

    char* vga = 0xb8000;
    vga[offset_from_vga] = 'S'; 
    vga[offset_from_vga+1] = 0x0f; // white text on black background
}
