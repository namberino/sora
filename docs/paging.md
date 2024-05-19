# Virtual memory

Example program:
```c
int main(int argc, char* argv[])
{
  return 0;
} 
```

When I `objdump` this program, the program starts at address `0x80482a0`, which is around 128MB into address space. Even though this program runs perfectly in a machine with < 128MB of RAM.

Paging allows a process to access more virtual memory than the physical RAM. If the RAM is full, the CPU could swap the pages that have not been used recently to disk and free up memory for other pages that needs it.

The program is reading and writing to a virtual address space. Some parts of the virtual address space maps to the physical memory, some parts don't

Trying to access the unmapped part will result in the processor raising a *page fault* error, the OS catches it, and (in POSIX systems) delivers a SIGSEGV signal closely followed by SIGKILL

It means that compilers can produce a program that relies on the code being at an exact location in memory, every time it is run. With virtual memory, the process thinks it is at, for example, `0x080482a0`, but actually it could be at physical memory location `0x1000000`. And processes cannot accidentally (or deliberately) trample other processes' data or code

Virtual memory is dependent on hardware support. x86 has support, it's called the MMU (Memory Management Unit). It handles all memory mappings due to segmentation and paging, forming a layer between the CPU and memory (it's part of the CPU)

# Concreting virtual memory

Virtual memory is an abstract concept so we need to implement it in someway, both segmentation and paging are valid ways to do this. 

Segmentation is becoming obsolete and paging is the newer, better alternative for x86 architectures

# Paging

Paging works by splitting the virtual address space into blocks called pages, which are usually *4KB* in size. Pages can then be mapped on to frames (equally sized blocks of physical memory)

## Page entries

Each process normally has a different set of page mappings, so that virtual memory spaces are independent of each other

In the x86 architecture (32-bit) pages are fixed at 4KB in size. Each page has a corresponding `descriptor word`, which tells the processor which frame it is mapped to

Pages and frames must be aligned on 4KB boundaries (4KB being 0x1000 bytes). The least significant 12 bits of the 32-bit word must always be 0. The processor use these bits for storing information about the page (present or not, kernel-mode or user-mode, etc.)

Layout of this word (page table entry format): 

![](../../img/page-table-entry-format.png)

Format information:
- `P`: Set if the page is present in memory
- `R/W`: If set, that page is writeable. If unset, the page is read-only. This does not apply when code is running in kernel-mode (unless a flag in cr0 is set)
- `U/S`: If set, this is a user-mode page. Else it is a kernel-mode page. User-mode code cannot write to or read from kernel-mode pages
- `RSVD` (Reserved): These are used by the CPU internally and cannot be trampled 
- `A`: Set if page has been accessed (gets set by the CPU)
- `D`: Set if page has been written to (dirty)
- `AVAIL`: These 3 bits are unused and available for kernel-use
- `Frame address`: The high 20 bits of the frame address in physical memory

## Page directory

To generate a table mapping of each 4KB page to 1 32-bit descriptor over a 4GB address space requires 4MB of memory

4MB seems like a large overhead. Compared to 4GB though, it's not much, but on systems with lower RAM, like 16MB, it's going to be a problem

We need something that will take up an amount of space proportionate to the amount of RAM you have

Intel CPU uses a 2-tier system: The CPU gets told about a page directory, which is a 4KB large table, each entry of which points to a page table. The page table is 4KB in size, each entry is a page table entry

This way, The entire 4GB address space can be covered with the advantage that if a page table has no entries, it can be freed and it's present flag unset in the page directory

![page directory layout](../img/page-directory-layout.png)

## Enable paging

Very easy:
1. Copy the location of your page directory into the `cr3` register (this must be the physical address)
2. Set the `PG` bit in the `cr0` register (by `or-ing` with `0x80000000`)

# Page faults

When a process does something the MMU doesn't like, a page fault interrupt is thrown. Situations that can cause this:
- Reading from or writing to an area of memory that is not mapped (page entry/table's 'present' flag is not set)
- The process is in user-mode and tries to write to a read-only page
- The process is in user-mode and tries to access a kernel-only page
- The page table entry is corrupted (the reserved bits have been overwritten)

Page fault interrupt number is **14**, this throws an error code:
- Bit 0: If set, the fault was not because the page wasn't present. If unset, the page wasn't present
- Bit 1: If set, the operation that caused the fault was a write, else it was a read
- Bit 2: If set, the processor was running in user-mode when it was interrupted. Else, it was running in kernel-mode
- Bit 3: If set, the fault was caused by reserved bits being overwritten
- Bit 4: If set, the fault occurred during an instruction fetch

# Memory management

When the kernel is sufficiently booted, we will have a kernel heap active and operational. The way we code heaps, though, usually requires that virtual memory is enabled. So we need a simple alternative to allocate memory before the heap is active

As we're allocating quite early on in the kernel bootup, we can make the assumption that nothing that is `kmalloc()'d` will ever need to be `kfree()'d`. So we can just have a pointer (placement address) to some free memory that we pass back to the requestee then increment (this pointer will keep growing)

# Page alignment

If you have memory that is paged (swapped in and out of disk) or cached then arranging blocks such as structures or arrays so that they don't cross a boundary speeds up processing because it ensures the memory doesn't need to be swapped or cache lines filled when the code accesses different parts of the block.

Page alignment is also helpful when protecting different parts of memory from being accessed by non-privileged processes as the protection hardware only has a limited number of registers so whole pages are protected rather than individual bytes or words.

# Frame allocation

To map a page to a frame, we need some way of finding a free frame. 

We could just maintain a massive array of 1's and 0's, but that would be extremely wasteful as we don't need 32-bits just to hold 2 values, we can do that with 1 bit. So by using a [bit array](https://en.wikipedia.org/wiki/Bit_array), we will be using 32 times less space

The bit array will have 3 functions: set, test, clear
