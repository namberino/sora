; ensures we jump straight into the kernel's entry function
[bits 32]
[extern main] ; define calling point, this must have same name as kernel.c 'main' function, the linker will look here and substitute with the address
call main ; calls the C function, the linker will know where it is placed in memory
jmp $ ; hang when we return from the kernel
