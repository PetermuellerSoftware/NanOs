#include <stdint.h>
#include <stdbool.h>

/*  Segment structure
 * 
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *  +- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --+-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -+
 *  | base 24-31           |  Flags    |limit16-19 | Access byte           | base 16-23           |
 *  +----------------------------------------------+----------------------------------------------+
 *  |         base 0 -15                           |            limit -15                         |
 *  +----------------------------------------------+----------------------------------------------+
 * 
 *  Flags
 *  20 - not used
 *  21 - L  Large only x64
 *  22 - Sz Size  (0=16 bit, 1=32 bit)
 *  23 - G  granularity (0=1b-1Mb, 1=4kb-4GB)
 * 
 *  Access byte
 *  08 - Ac Accessed, set from CPU when used in segment register
 *  09 - RW ReadWrite (Readable for Code Segments, writeable for data segments)
 *  10 - DC Dircetion bit ( 0= grows up, 1= grows down)
 *  11 - Ex Exectuable (0= Data, 1= Code)
 *  12 - S  Descritpor Type (1= code/data, 0= System)
 *  13 - Privl
 *  14 -    Privilege (ring level)
 *  15 - P  Present (1=valied, 0 not valid)
 */

#define GDT_FLAG_LARGE 1 << 5
#define GDT_FLAG_SIZE 1 << 6
#define GDT_FLAG_GRANULARITY 1 << 7

#define GDT_ACCESS_RW 1 << 1
#define GDT_ACCESS_DIRECTION_DOWN 1 << 2
#define GDT_ACCESS_EXECUTABLE 1 << 3
#define GDT_ACCESS_CODE_DATA 1 << 4
#define GDT_ACCESS_PRESENT 1 << 7


/* keeps information about the gdt_entry to build
 */
typedef struct 
{
    uint32_t base;
    uint32_t limit;  // dont use mor than 20 bit
    
    uint8_t flags;    
    uint8_t access;
    uint8_t ring;
} gdt_source; 

struct gdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));
 

typedef struct gdt_ptr_struct gdt_ptr_t;

void GDT_createEntry(uint8_t* gdt_entry, gdt_source source)
{
    gdt_entry[0] = source.limit;
    gdt_entry[1] = source.limit >> 8;
    gdt_entry[2] = source.base;
    gdt_entry[3] = source.base >> 8;
    gdt_entry[4] = source.base >> 16;
    gdt_entry[5] = (source.access & 0x9F) | ( (source.ring & 0x03) << 5);
    gdt_entry[6] = ((source.limit >> 16) & 0x0F) | (source.flags & 0xF0); 
    gdt_entry[7] = source.base >> 24;    
}