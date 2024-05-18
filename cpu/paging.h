#ifndef PAGING_H
#define PAGING_H

#include "ports.h"
#include "isr.h"
#include <stdint.h>

// reference docs/paging.md to understand each part of the code

typedef struct page
{
   uint32_t present : 1; // page present in memory (bit field: 1 bit)
   uint32_t read_write : 1; // read-only if 0, read-write if 1 (bit field: 1 bit)
   uint32_t user : 1; // supervisor level only if 0 (bit field: 1 bit)
   uint32_t accessed : 1; // has the page been accessed since last refresh (bit field: 1 bit)
   uint32_t dirty : 1; // has the page been written to since last refresh (bit field: 1 bit)
   uint32_t unused : 7; // amalgamation of unused and reserved bits (bit field: 7 bits)
   uint32_t frame : 20; // frame address (shifted right 12 bits) (bit field: 20 bits)
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    page_table_t* tables[1024]; // array of pointers to page tables
    uint32_t tables_physical_addresses[1024]; // array of pointers to the page tables but gives the physical addresses (for loading into the cr3 register)
    uint32_t physical_address; // physical address of tables_physical_address, used for when we get the kernel heap allocated (and the directory may be in a different location in virtual memory)
} page_directory_t;

void initialize_paging(); // set up page directories, etc. and enable paging
void switch_page_directory(page_directory_t* new_page_dir); // loads the specified page directory into the cr3 register

// get pointer to the page required
// if make == 1, create the page table that contains this page if it is not created yet
page_t* get_page(uint32_t address, int make, page_directory_t* directory);

void page_fault_handler(register_t regs);

#endif
