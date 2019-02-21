#include "GDT.h"

// space to store the gdt in
uint8_t gdt[6*8];

gdt_ptr_t gdtr;

extern void _flush_gdr( gdt_ptr_t* ); 


void kernel_init_gdt() 
{
    
    #define GDT_MAX_SIZE 6
    gdt_source source[ GDT_MAX_SIZE ];
    
    source[0] = (gdt_source) {.base=0, .limit=0, .ring=0, .flags=0, .access=0}; // Selector 0x00 cannot be used

// kernel space
    source[1] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .ring=0, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT }; // Selector 0x08 kernel code
    source[2] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .ring=0, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT }; // Selector 0x10 kernel data
 
 // user space 
   source[3] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .ring=3, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT }; // Selector 0x18 user code
    source[4] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .ring=3, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT }; // Selector 0x20 user data
       
    GDT_createEntry( gdt, source[0] );
    GDT_createEntry( gdt+8, source[1] );
    GDT_createEntry( gdt+16, source[2] );
    GDT_createEntry( gdt+24, source[3] );
    GDT_createEntry( gdt+32, source[4] );
    
    
    gdtr.base = (uint32_t) gdt;
    gdtr.limit = sizeof(gdt);
    
    _flush_gdr( &gdtr);

    
}