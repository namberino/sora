#include "timer.h"

volatile uint32_t tick = 0;

// timer handler
static void timer_callback(registers_t* regs)
{
    tick++;
    UNUSED(regs);
    
    // kprint("Tick: ");
    
    // char tick_ascii[256];
    // int_to_ascii(tick, tick_ascii);
    // kprint(tick_ascii);
    // kprint("\n");
}

void init_timer(uint32_t frequency)
{
    // initalize PIT handler
    register_int_handler(IRQ0, timer_callback);

    // get PIT value for clock at 1193180 Hz
    uint32_t divisor = 1193180 / frequency;

    // divisor has to be sent byte-wise, so split into upper and lower bytes
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
    
    // send the command 
    port_byte_out(0x43, 0x36); // send command byte to command port (0x36 sets PIT to repeating mode so that when the divisor counter reaches zero it's automatically refreshed)
    
    // set divisor value
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}

// function for setting the PIT frequency after initialization
void set_pit2(uint32_t frequency)
{
    // get PIT value for clock at 1193180 Hz
    uint32_t divisor = 1193180 / frequency;

    // divisor has to be sent byte-wise, so split into upper and lower bytes
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    // send the command 
    port_byte_out(0x43, 0xb6); // send command byte to command port (0xb6 is rate generator mode, used for pwm)
    
    // set divisor value (0x42 to set it on channel 2 of PIT)
    port_byte_out(0x42, low);
    port_byte_out(0x42, high);
}

// function for halting the timer for a certain amount of ticks
void halt_timer(uint32_t halt_ticks)
{
    uint32_t end_tick = tick + halt_ticks;

    while (tick < end_tick)
    {
        asm volatile("nop");
    }
}
