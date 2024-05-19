#include "paging.h"

page_directory_t* kernel_directory = 0; // kernel's page directory
page_directory_t* current_directory = 0; // current page directory

// bitset of frames - used or free
uint32_t* frames;
uint32_t nframes;

void initialize_paging()
{

}

void switch_page_directory(page_directory_t* new_page_dir)
{

}

page_t* get_page(uint32_t address, int make, page_directory_t* directory)
{

}

void page_fault_handler(registers_t regs)
{

}
