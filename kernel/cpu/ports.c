// read bytes from specific port
unsigned char port_byte_in(unsigned short port)
{
    /*
    Inline assembly syntax:
    source and destination registers are switched from nasm
    '"=a" (result)'; set '=' the C variable '(result)' to the value of register e'a'x
    '"d" (port)': map the C variable '(port)' into e'd'x register
    */
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    // nothing is returned so no '='
    // since there are two variables in the input area and none in the 'return' area so 2 commas there
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

unsigned short port_word_in(unsigned short port)
{
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_word_out(unsigned short port, unsigned short data)
{
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
