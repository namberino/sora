#include "screen.h"

// this uses vga ports to get current cursor position offset
// get high byte (byte 14) and low byte (byte 15)
int get_cursor_offset()
{
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; // shift to high byte
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);

    return offset * 2; // position * size of cell
}

// similar to get cursor offset but for writing data
void set_cursor_offset(int offset)
{
    offset /= 2;

    port_byte_out(REG_SCREEN_CTRL, 14); // open port to send
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8)); // send high byte
    port_byte_out(REG_SCREEN_CTRL, 15); // open port to send
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff)); // send low byte
}

// each rows contain a maximum number of cols
// add up with the current col to get the current cell position
// each cells take up 2 bytes
int get_offset(int col, int row)
{
    return 2 * (row * MAX_COLS + col);
}

// each rows contain a max number of cols and each cells take up 2 bytes
int get_offset_row(int offset)
{
    return offset / (2 * MAX_COLS); 
}

// calculate the byte offset of row 
// subtract the byte offset of the start of the row from the given offset to get the byte offset within the row
// divide byte offset within row by 2 to get col number
int get_offset_col(int offset)
{
    return (offset - (get_offset_row(offset) * 2 * MAX_COLS)) / 2;
}

int print_char(char c, int col, int row, char attr)
{
    uint8_t* video_mem = (uint8_t*)VIDEO_ADDRESS;
    if (!attr) // assign default color attribute
        attr = DEFAULT_COLOR;

    // print red E if coordinates aren't right
    if (col >= MAX_COLS || row >= MAX_ROWS)
    {
        video_mem[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        video_mem[2 * MAX_COLS * MAX_ROWS - 1] = ERROR_COLOR_2;

        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else // if negative coordinates
        offset = get_cursor_offset();

    if (c == '\n')
    {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    }
    else if (c == 0x08) // backspace handling (not advance offset when printing backspace)
    {
        video_mem[offset] = ' ';
        video_mem[offset + 1] = attr;
    }
    else
    {
        video_mem[offset] = c;
        video_mem[offset + 1] = attr;
        offset += 2;
    }

    // check if offset is over the screen size and scroll
    // copy the rows upwards and clear the text on the last line
    if (offset >= MAX_ROWS * MAX_COLS * 2)
    {
        for (int i = 1; i < MAX_ROWS; i++)
            mem_copy((uint8_t*)(get_offset(0, i) + VIDEO_ADDRESS), (uint8_t*)(get_offset(0, i - 1) + VIDEO_ADDRESS), MAX_COLS * 2); // copy video memory of each rows

        // clear text on the last line
        char* last_line = (char*)(get_offset(0, MAX_ROWS - 1) + (uint8_t*)VIDEO_ADDRESS);
        for (int i = 0; i < MAX_COLS * 2; i++) 
            last_line[i] = 0;
        
        offset -= 2 * MAX_COLS; // move offset back by a row
    }

    set_cursor_offset(offset);
    return offset;
}

void kprint_color_at(char* message, int col, int row, char color)
{
    int offset;

    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
    {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int i = 0;
    while (message[i] != 0)
    {
        offset = print_char(message[i++], col, row, color);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint_color(char* message, char color)
{
    kprint_color_at(message, -1, -1, color);
}

// print message at specified location
// if rol and col is negative, use current location
void kprint_at(char* message, int col, int row)
{
    kprint_color_at(message, col, row, DEFAULT_COLOR);
}

// print at current location
void kprint(char* message)
{
    kprint_color(message, DEFAULT_COLOR);
}

void kprint_backspace()
{
    int offset = get_cursor_offset() - 2;
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    print_char(0x08, col, row, CMD_COLOR);
}

void clear_screen()
{
    int screen_size = MAX_COLS * MAX_ROWS;
    uint8_t* screen = (uint8_t*)VIDEO_ADDRESS;

    // modifying every 2 bytes
    for (int i = 0; i < screen_size; i++)
    {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = CLEAR_SCREEN_COLOR;
    }

    set_cursor_offset(get_offset(0, 0));
}
