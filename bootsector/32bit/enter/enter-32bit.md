# Entering 32-bit protected mode

- Once GDT and GDT descriptor is prepared in our bootsector, we can enter 32-bit protected mode

# Steps

- Disable interrupt using the `cli` instruction (clear interrupt, the CPU will ignore future interrupts that may happen, at least until interrupts are later enabled)
- We can pass the GDT descriptor to `lgdt` to tell the CPU about our GDT
```asm
lgdt [gdt_descriptor]
```

- By setting the 1st bit on the CPU control register `cr0`, we can enter. Cannot set this bit directly on the register, we need to load it into a general purpose register, set the bit, then store it back to `cr0` (Use `or` instruction to include certain bits)
```asm
mov eax, cr0 ; make the switch to protected mode
or eax, 0x1  ; set 1st bit to 1
mov cr0, eax ; update cr0
```

# Flushing the pipeline

- Now the CPU is almost in 32-bit protected mode
- Since modern processors use *pipelining* (allows them to process different stages of an instruction execution in parallel for single CPU), this allows them to execute instructions in less time
- We don't usually need to worry about CPU internals like pipelining when programming the CPU. However, when switching CPU modes, there's a risk of the CPU processing some stages of an instruction's execution in the wrong mode

- Immediately after **instructing** the CPU to enter 32-bit mode, force the CPU to finish any jobs in its pipeline (so we can make sure all instructions execute in the right mode and the CPU won't crash)
- We can do this by using far jumping to another segment as CPU can't really pre-fetch and pipeline branching instructions like `jmp` and `call`
- A far jump can force the CPU to flush the pipeline (complete all instructions currently in different stages of the pipeline)
- Far jump usually need to be provided with a target segment:
```asm
jmp <segment>:<address offset>
```

# Far jumping

- For this far jump, we need to be careful about where we land
- If we have a `start_protected_mode` label in our code (the beginning of the 32-bit code), a jump to this might not be sufficient to flush the pipeline, and that might be dangerous because the current code segment (`cs`) is not in valid protected mode yet

- Update the `cs` register to the offset of the code segment descriptor of GDT
- Since segment descriptors are 8-byte each, and the code descriptor is the 2nd descriptor in our GDT, its offset will be 0x8, so this value is what we must set our code segment register to
- This is a far jump, it will cause the CPU to automatically update the `cs` register to the target segment

- We can the assembler to calculate these segment descriptor offsets and store them as constants `CODE_SEG` and `DATA_SEG`:
```asm
jmp CODE_SEG:start_protected_mode

[bit 32]
start_protected_mode: ; now we are securely in protected mode
```

- `bit 32` directive will encode in 32-bit mode instructions
- Now we can make use of `eax` to set the control bit
- Now we are in 32-bit protected mode
- What we should do now is update all other segment registers so they point to the 32-bit data segment rather than the invalid real mode segments
- We should also update the position of the stack


