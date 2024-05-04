; steps to enter 32-bit PM
; - Disable interrupts
; - Set a bit on the CPU control register cr0
; - Flush the CPU pipeline by issuing a carefully crafted far jump
; - Update all the segment registers
; - Update the stack
; - Call to a well-known label which contains the first useful code in 32 bits
