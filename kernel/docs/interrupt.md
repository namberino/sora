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

# Reference

[IDT](https://web.archive.org/web/20160326064709/http://jamesmolloy.co.uk/tutorial_html/4.-The%20GDT%20and%20IDT.html)
[OSdev wiki - IDT](https://wiki.osdev.org/Interrupt_Descriptor_Table)
[OSdev wiki - ISR](https://wiki.osdev.org/Interrupt_Service_Routine)
