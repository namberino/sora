#include "types.h"
#include "timer.h"
#include "../kernel/util.h"
#include "../drivers/screen.h"
#include "isr.h"

u32 tick = 0;

// timer handler (currently prints current tick count to screen)
static void timer_callback(registers_t regs)
{
    tick++;
    kprint("Tick: ");
    
    char tick_ascii[256];
    int_to_ascii(tick, tick_ascii);
    kprint(tick_ascii);
    kprint("\n");
}

void init_timer(u32 frequency)
{
    // initalize PIT handler
    register_int_handler(IRQ0, timer_callback);

    // get PIT value for clock at 1193180 Hz
    u32 divisor = 1193180 / frequency;

    // divisor has to be sent byte-wise, so split into upper and lower bytes
    u8 low  = (u8)(divisor & 0xFF);
    u8 high = (u8)( (divisor >> 8) & 0xFF);
    
    // send the command 
    port_byte_out(0x43, 0x36); // send command byte to command port (0x36 sets PIT to repeating mode so that when the divisor counter reaches zero it's automatically refreshed)
    
    // set divisor value
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
