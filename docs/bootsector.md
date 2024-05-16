# The current bootsector

The current bootsector is not very big, it's a few hundred lines of codes that can do a couple of things:
- Read from disk
- Set up GDT
- Set up the entry point of the kernel
- Switch to 32-bit real mode

# Switch to GRUB

Switching to GRUB will be a better option in the long run as the OS keep getting bigger and more complicated. GRUB will do a lot of the bootloader heavy lifting for us.
