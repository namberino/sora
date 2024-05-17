#include "speaker.h"

// reference https://wiki.osdev.org/PC_Speaker
// bit 1 and bit 0 will need to be set to use the speaker
static void play_sound(uint32_t frequency)
{
    set_pit2(frequency);

    uint8_t temp = port_byte_in(0x61);
    if (temp != (temp | 3)) // if bit 1 and bit 0 is not set
        port_byte_out(0x61, temp | 3);
}

static void no_sound()
{
    uint8_t temp = port_byte_in(0x61) & 0xfc;
    port_byte_out(0x61, temp);
}

void beep()
{
    asm volatile("sti"); // enable interrupts
    play_sound(1000); // 1000Hz sound
    halt_timer(5); // play for 5 ticks
    no_sound();
    set_pit2(50);
}
