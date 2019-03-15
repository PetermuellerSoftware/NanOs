#include <stdint.h>

#include "MemoryController.h"
#include "multiboot_information.h"
#include "vga.h"

/* We have 4 GiB: 2 ^ (10 + 10 + 10 + 2)
 * in pages of 4 kiB 2 ^ ( 10 + 2 )
 * so we need 2 ^ 20 bits to track all possible pages
 * 
 */
uint32_t physical_pages[32768]; 

extern vga vga;

/*inline void getBlockPosisition (uint32_t* position, uint32_t &block, uint32_t &bit)
{
    const alignedPosition = position & 0xFFFFF000; // align to 64 
}*/

extern "C" void initializePhysicalBlocks()
{
    int i;
    if (!(multiboot_information->flags & MULTIBOOT_FLAG_MMAP) ) {
        vga.writestring("fatal: no memory map information \n"); 
        for (i=0;;);
    }
    vga.writestring("Initialize pyhsical pages: \n");
    vga.writestring("\taddress: 0x"); vga.writeHex(multiboot_information->mmap_address);
    vga.writestring("\n\tentries: 0x"); vga.writeHex(multiboot_information->mmap_length);
    vga.writestring("\n");
}

uint32_t get_physical_block ()
{
    
}

void free_physical_block ()
{

}
