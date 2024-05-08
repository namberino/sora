#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/string.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

// scancode (reference https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html)
#define BACKSPACE 0x0E
#define ENTER 0x1C
#define LSHIFT_DOWN 0x2A
#define LSHIFT_UP 0xAA
#define RSHIFT_DOWN 0x36
#define RSHIFT_UP 0xB6
#define CAPSLOCK_DOWN 0x3A
#define CAPSLOCK_UP 0xBA
#define SCANCODE_MAX 57
int cap = 0;

static char key_buffer[256]; // buffer

const char* scancode_name[] = { 
    "ERROR", "ESC", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", 
    "-", "=", "BACKSPACE", "TAB", "Q", "W", "E", "R", "T", "Y", "U", 
    "I", "O", "P", "[", "]", "ENTER", "LCTRL", "A", "S", "D", "F", 
    "G", "H", "J", "K", "L", ";", "'", "`", "LSHIFT", "\\", "Z", 
    "X", "C", "V", "B", "N", "M", ",", ".", "/", "RSHIFT", "KEYPAD *", 
    "LALT", "SPACE"
};

const char scancode_ascii[] = { 
    '?', '?', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', 
    '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', 
    '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 
    ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', 
    ',', '.', '/', '?', '?', '?', ' '
};

static void keyboard_callback(registers_t regs)
{
    // PIC leaves the scancodes in port 0x60
    u8 scancode = port_byte_in(0x60);

    if (scancode == LSHIFT_UP || scancode == RSHIFT_UP || scancode == CAPSLOCK_UP)
        cap = 0;

    if (scancode > SCANCODE_MAX)
        return;

    if (scancode == BACKSPACE)
    {
        if (key_buffer[0] != '\0')
        {
            backspace(key_buffer);
            kprint_backspace();
        }
    }
    else if (scancode == ENTER)
    {
        kprint("\n");
        user_input(key_buffer); // kernel-controlled function 
        key_buffer[0] = '\0'; // clear key buffer
    }
    else if (scancode == LSHIFT_DOWN || scancode == RSHIFT_DOWN || scancode == CAPSLOCK_DOWN)
    {
        if (cap == 0)
            cap = 1;
    }
    else
    {
        char letter = scancode_ascii[(int)scancode];

        if (cap == 1)
            letter = char_upper(letter);

        append(key_buffer, letter); // append to key buffer

        char str[2] = {letter, '\0'};
        kprint(str); // print the letter to screen
    }
    UNUSED(regs);
    // int_to_ascii(scancode, scancode_ascii);
    // kprint("Keyboard scancode: ");
    // kprint(scancode_ascii);
    // kprint(", ");
    // print_letter(scancode);
    // kprint("\n");
}

void init_keyboard()
{
    register_int_handler(IRQ1, keyboard_callback);
}

/*
void print_letter(u8 scancode)
{
    switch (scancode)
    {
        case 0x0:
            kprint("ERROR");
            break;

        case 0x1:
            kprint("ESC");
            break;

        case 0x2:
            kprint("1");
            break;

        case 0x3:
            kprint("2");
            break;

        case 0x4:
            kprint("3");
            break;

        case 0x5:
            kprint("4");
            break;

        case 0x6:
            kprint("5");
            break;

        case 0x7:
            kprint("6");
            break;

        case 0x8:
            kprint("7");
            break;

        case 0x9:
            kprint("8");
            break;

        case 0x0A:
            kprint("9");
            break;

        case 0x0B:
            kprint("0");
            break;

        case 0x0C:
            kprint("-");
            break;

        case 0x0D:
            kprint("+");
            break;

        case 0x0E:
            kprint("BACKSPACE");
            break;

        case 0x10:
            kprint("Q");
            break;

        case 0x11:
            kprint("W");
            break;

        case 0x12:
            kprint("E");
            break;

        case 0x13:
            kprint("R");
            break;

        case 0x14:
            kprint("T");
            break;

        case 0x15:
            kprint("Y");
            break;

        case 0x16:
            kprint("U");
            break;

        case 0x17:
            kprint("I");
            break;

        case 0x18:
            kprint("O");
            break;

        case 0x19:
            kprint("P");
            break;

		case 0x1A:
			kprint("[");
			break;

		case 0x1B:
			kprint("]");
			break;

		case 0x1C:
			kprint("ENTER");
			break;

        case 0x1D:
            kprint("LCTRL");

        case 0x1E:
			kprint("A");
			break;

		case 0x1F:
			kprint("S");
			break;

        case 0x20:
            kprint("D");
            break;

        case 0x21:
            kprint("F");
            break;

        case 0x22:
            kprint("G");
            break;

        case 0x23:
            kprint("H");
            break;

        case 0x24:
            kprint("J");
            break;

        case 0x25:
            kprint("K");
            break;

        case 0x26:
            kprint("L");
            break;

        case 0x27:
            kprint(";");
            break;

        case 0x28:
            kprint("'");
            break;

        case 0x29:
            kprint("`");
            break;

		case 0x2A:
			kprint("LSHIFT");
			break;

        case 0x2B:
            kprint("\\");
            break;

        case 0x2C:
			kprint("Z");
			break;

		case 0x2D:
			kprint("X");
			break;

		case 0x2E:
			kprint("C");
			break;

		case 0x2F:
			kprint("V");
			break;

        case 0x30:
            kprint("B");
            break;

        case 0x31:
            kprint("N");
            break;

        case 0x32:
            kprint("M");
            break;

        case 0x33:
            kprint(",");
            break;

        case 0x34:
            kprint(".");
            break;

        case 0x35:
            kprint("/");
            break;

        case 0x36:
            kprint("RSHIFT");
            break;

        case 0x37:
            kprint("KEYPAD *");
            break;

        case 0x38:
            kprint("LALT");
            break;

        case 0x39:
            kprint("SPACE");
            break;

        // keyup events corresponds to keydown + 0x80
        // there may be some scancode we haven't implemented yet or a control / escape sequence
        default:
            if (scancode <= 0x7f)
            {
                kprint("Unknown keydown");
            }
            else if (scancode <= 0x39 + 0x80)
            {
                kprint("Keyup ");
                print_letter(scancode - 0x80);
            }
            else
            {
                kprint("Unknown keyup");
            }
            break;
    }
}
*/
