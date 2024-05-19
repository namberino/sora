#include "paging.h"

// reference docs/paging.md to understand frame allocation

// macros for the bit array algorithm
#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

page_directory_t* kernel_directory = 0; // kernel's page directory
page_directory_t* current_directory = 0; // current page directory

// bitset of frames - used or free
uint32_t* frames;
uint32_t nframes;

extern uint32_t free_mem_address;

// set a bit in the frames bit array
static void set_frame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] |= (0x1 << offset);
}

// clear a bit in the frames bit array
static void clear_frame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    frames[index] &= ~(0x1 << offset);
}

static uint32_t test_frame(uint32_t frame_address)
{
    uint32_t frame = frame_address / 0x1000;
    uint32_t index = INDEX_FROM_BIT(frame);
    uint32_t offset = OFFSET_FROM_BIT(frame);
    return (frames[index] & (0x1 << offset));
}

// find the first free frame
static uint32_t find_first_free_frame()
{
    for (uint32_t i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {
        if (frames[i] != 0xffffffff) // not free, exit early
        {
            // at least 1 bit is free here
            for (uint32_t j = 0; j < 32; j++)
            {
                uint32_t bit_to_test = 0x1 << j;

                if (!(frames[i] & bit_to_test))
                    return i * 4 * 8 + j;
            }
        }
    }
}

void allocate_frame(page_t* page, int is_kernel, int is_writeable)
{
    if (page->frame != 0) // frame already allocated
    {
        return;
    }
    else
    {
        uint32_t index = find_first_free_frame(); // get index of first free frame

        if (index == (uint32_t) -1)
        {
            kprint_color("No free frame\n", ERROR_COLOR_1);
            halt();
        }

        set_frame(index * 0x1000); // set frame to the found free frame (we can now use it)

        // modify page status
        page->present = 1;
        page->read_write = (is_writeable) ? 1 : 0;
        page->user = (is_kernel) ? 0 : 1;
        page->frame = index; // set frame address to index of found free frame
    }
}

void free_frame(page_t* page)
{
    uint32_t frame;
   if (!(frame = page->frame)) // if given page didn't have an allocated frame
   {
       return;
   }
   else
   {
       clear_frame(frame); // free page
       page->frame = 0x0; // set page frame address to nothing
   }
}

// paging initialization and setup
void switch_page_directory(page_directory_t* new_page_directory) // switch to specified page directory
{
    uint32_t cr0;
    current_directory = new_page_directory;

    // load physical page table addresses to cr3
    asm volatile("mov %0, %%cr3" : : "r" (&new_page_directory->tables_physical_addresses));
    
    // enable paging and flush the page directory cache
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}

page_t* get_page(uint32_t address, int make, page_directory_t* directory)
{
    address /= 0x1000; // turn address to index

    // find page table containing this address
    uint32_t table_index = address / 1024;

    if (directory->tables[table_index]) // if this table is already assigned
    {
        // get pointer to page at specified address
        return &directory->tables[table_index]->pages[address % 1024];
    }
    else if (make) // if not assigned and create page table is set
    {
        // uses kmalloc_ap to allocate a page aligned memory block (gets its physical location)
        // physical location is stored in tables_physical_addresses
        // virtual location is stored in tables
        uint32_t temp_table;
        directory->tables[table_index] = (page_table_t*) kmalloc_ap(sizeof(page_table_t), &temp_table);
        mem_set((uint8_t*) directory->tables[table_index], 0, 0x1000);
        // store physical location
        directory->tables_physical_addresses[table_index] = temp_table | 0x7; // set bits: present, read_write, user (user-accessible)

        return &directory->tables[table_index]->pages[address % 1024];
    }
    else // page not assigned and create page talbe is not set
    {
        return 0;
    }
}

void page_fault_handler(registers_t* reg)
{
    // faulting address is stored in cr2
    uint32_t faulting_address;
    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

    // get error code (for details on what happened)
    int present = !(reg->err_code & 0x1); // page not present?
    int read_write = reg->err_code & 0x2; // write operation?
    int user = reg->err_code & 0x4; // processor was in user-mode?
    int reserved = reg->err_code & 0x8; // overwrote reserved bits of page entry?
    int id = reg->err_code & 0x10; // caused by an instruction fetch? (e.g: execute code in a page that has "execute disable" bit set)
    
    kprint_color("Page fault: ", ERROR_COLOR_1);
    if (present)
        kprint_color("NOT PRESENT ", ERROR_COLOR_1);
    if (read_write)
        kprint_color("READ ONLY ", ERROR_COLOR_1);
    if (user)
        kprint_color("IN USER MODE ", ERROR_COLOR_1);
    if (reserved)
        kprint_color("OVERWROTE RESERVED BITS ", ERROR_COLOR_1);
    if (id)
        kprint_color("INSTRUCTION FETCH ", ERROR_COLOR_1);

    char* faulting_address_ascii;
    hex_to_ascii((int)faulting_address, faulting_address_ascii);
    kprint_color("\nAt address 0x", ERROR_COLOR_1);
    kprint_color(faulting_address_ascii, ERROR_COLOR_1);
    kprint("\n");
    // halt();
}

/*
First, this function will create a frame bit array and set everything to 0
Second, this function will then allocates space (which are page-aligned) for a page directory
Third, this function will then allocates frames so that any page access will map to the frame with the same linear address (identity mapping)
Fourth, this function will then registers an interrupt handler for page faults and enable paging

Identity mapping is done to a small section of the address space so the kernel can continue to run
*/
void initialize_paging()
{
    // we'll assume the physical memory size is 16mb
    uint32_t mem_end_page = 0x1000000;
    nframes = mem_end_page / 0x1000; // number of frames relative to physical memory size
    frames = (uint32_t*) kmalloc(INDEX_FROM_BIT(nframes)); // pointer to first address of the allocated frames
    mem_set((uint8_t*) frames, 0, INDEX_FROM_BIT(nframes));

    // page directory
    kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t)); // allocate a location in memory for kernel directory
    mem_set((uint8_t*) kernel_directory, 0, sizeof(page_directory_t)); // initialize kernel directory with 0s
    current_directory = kernel_directory;

    // need to identity map (physical address = virtual address) from 0x0 to end of used memory to be able to access this trasnparently,
    // as if paging wasn't enabled
    int i = 0;
    while (i < free_mem_address)
    {
        // kernel code is readable but not writeable from userspace so 2 0s
        allocate_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    register_int_handler(14, (isr_t) page_fault_handler); // register page fault interrupt handler
    switch_page_directory(kernel_directory); // enable paging for specified directory
}
