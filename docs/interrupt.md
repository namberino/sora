# Interrupts

- Handled on a vector, with entries which are similar to those of the GDT
- We program the IDT in C instead of assembly

# Interrupt Description Table

- Tells the processor where to find handlers for each interrupt (similar to GDT)
- An array of entries, each one corresponding to an interrupt number
- The processor will panic and reset if there's an interrupt with no entry for it (a null entry is fine)

- IDT entries are stored in IDT gates (256 of these)
- The actual IDT structure that the BIOS will load: IDT register which is just a memory address and a size (similar to the GDT register)

# First 32 interrupts

- Special, CPU-dedicated interrupts:

```
    0 - Division by zero exception
    1 - Debug exception
    2 - Non maskable interrupt
    3 - Breakpoint exception
    4 - 'Into detected overflow'
    5 - Out of bounds exception
    6 - Invalid opcode exception
    7 - No coprocessor exception
    8 - Double fault (pushes an error code)
    9 - Coprocessor segment overrun
    10 - Bad TSS (pushes an error code)
    11 - Segment not present (pushes an error code)
    12 - Stack fault (pushes an error code)
    13 - General protection fault (pushes an error code)
    14 - Page fault (pushes an error code)
    15 - Unknown interrupt exception
    16 - Coprocessor fault
    17 - Alignment check exception
    18 - Machine check exception
    19-31 - Reserved
```

# Interrupt Service Routine

- A software routine that hardware invokes in response to an interrupt (also known as an interrupt handler)

- When the processor receives an interrupt, it saves the contents of the essential registers
- It then calls an interrupt handler and then finally it restores the register contents stored

- We don't know which interrupt was the caller when a handler is called so we need to have different handler for every interrupt

- Some ISR push an error code onto the stack, others don't so we need to push dummy error code for those that don't so that the stack can be consistent

# Interrupt Request (IRQ)

- All devices that are interrupt-capable have a line connecting them to the **PIC** (Programmable Interrupt Controller)
- The PIC is the only device that is directly connected to the CPU's interrupt pin
- PIC is used as a multiplexer and it prioritises between interrupting devices
- All modern PC have 2 PICs (master and slave), serving 15 interrupt devices (one line is used to signal the slave PIC)

- Remapping the PIC: Change the interrupt number it delivers for each IRQ line
- Default mapping: 
  - IRQ 0..7 - INT 0x8..0xF
  - IRQ 8..15 - INT 0x70..0x77

- The master IRQ mappings conflict with the interrupt numbers used by our processor to signal exceptions and fault, so we need to remap the PICs so that the IRQs 0..15 correspond to ISRs 32..47 (31 is the last CPU-used ISR)

- PIC communicate via I/O ports
- The master PIC: command = 0x20 and data = 0x21
- The slave PIC: command = 0xA0 and data = 0xA1

- To remap the PIC, you have to do a full reinitialisation of them (code for this is pretty weird and obfuscated)

# Programmable Interval Timer (PIT)

- A chip connected to IRQ0
- Can interrupt processor at a user-defined rate (between 18.2Hz and 1.1931 MHz) 
- The PIT is the primary method used for implementing a system clock and the only method available for implementing multitasking (switch processes on interrupt)

- The PIT has an internal clock which oscillates at approximately 1.1931MHz
- This clock signal is fed through a frequency divider, to modulate the final output frequency
- The PIT has 3 channels (each with its own freq divider):
    - Channel 0 is the most useful. It's output is connected to IRQ0
    - Channel 1 is very un-useful and on modern hardware is no longer implemented. It used to control refresh rates for DRAM
    - Channel 2 controls the PC speaker
- We'll use channel 0

- Set the PIT so it interrupts us at regular intervals at frequency *f*
```
divisor = 1193180 Hz / frequency (in Hz)
```

- PIT has 4 registers in I/O space - 0x40-0x42 are the data ports for channels 0-2 respectively, and 0x43 is the command port

# Keyboard interrupt

- We can use PIC for keyboard interrupting, but PIC doesn't send an ASCII code for key pressed. It sends the scancode for the key-down and key-up events
- We need to translate those scancodes to get ASCII code

# Reference

> *Polling* is a method where a device continuously checks the status of another device to determine if it needs attention

[IDT](https://web.archive.org/web/20160326064709/http://jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html)
[OSdev wiki - IDT](https://wiki.osdev.org/Interrupt_Descriptor_Table)
[OSdev wiki - ISR](https://wiki.osdev.org/Interrupt_Service_Routine)
[OSdev wiki - PIC](https://wiki.osdev.org/PIC)
[Keyboard scancodes](https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html)
