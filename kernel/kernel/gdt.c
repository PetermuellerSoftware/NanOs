/*
    

    Diese Datei ist Teil von NanOs.

    NanOs ist Freie Software: Sie können es unter den Bedingungen
    der GNU General Public License, wie von der Free Software Foundation,
    Version 3 der Lizenz oder (nach Ihrer Wahl) jeder neueren
    veröffentlichten Version, weiter verteilen und/oder modifizieren.

    NanOs wird in der Hoffnung, dass es nützlich sein wird, aber
    OHNE JEDE GEWÄHRLEISTUNG, bereitgestellt; sogar ohne die implizite
    Gewährleistung der MARKTFÄHIGKEIT oder EIGNUNG FÜR EINEN BESTIMMTEN ZWECK.
    Siehe die GNU General Public License für weitere Details.

    Sie sollten eine Kopie der GNU General Public License zusammen mit diesem
    Programm erhalten haben. Wenn nicht, siehe <https://www.gnu.org/licenses/>.
*/
/*
|**********************************************************************;
* Project           : NanOs
*
* Author            : Peter Müller
*
* Date created      : 2019
*
|**********************************************************************;
*/


#include <stdint.h>

#include "kernel/GDT.h"



// space to store the gdt in
uint8_t gdt[6*8];

xdt_ptr_t gdtr;

extern void _flush_gdr( xdt_ptr_t* ); 


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

void kernel_init_gdt() 
{
    
    #define GDT_MAX_SIZE 5
    gdt_source source[ GDT_MAX_SIZE ];
    
// 0 descriptor
    source[0].base = 0;
    source[0].limit = 0;
    source[0].flags= 0;
    source[0].access = 0;
    source[0].ring = 0;
    
// kernel space code segemnt
    source[1].base=0;
    source[1].limit = 0xffffffff;
    source[1].flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE;
    source[1].access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT;
    source[1].ring=0;

// kernel space data segemnt    
    source[2].base=0;
    source[2].limit = 0xffffffff;
    source[2].flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE;
    source[2].access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT;
    source[2].ring=0;

 // user space code segment 
    source[3].base = 0, 
    source[3].limit = 0xffffffff, 
    source[3].flags = GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
    source[3].access = GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT, 
    source[3].ring =3;
    
 // user space data segment 
    source[4].base = 0, 
    source[4].limit = 0xffffffff, 
    source[4].flags = GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
    source[4].access = GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT,
    source[4].ring = 3;
       
    GDT_createEntry( gdt + (0 << 3), source[0] );   // 0
    GDT_createEntry( gdt + (1 << 3), source[1] );   // 0x08 kernel code
    GDT_createEntry( gdt + (2 << 3), source[2] );   // 0x10 kernel data
    GDT_createEntry( gdt + (3 << 3), source[3] );   // 0x18 user code
    GDT_createEntry( gdt + (4 << 3), source[4] );   // 0x20 user data
    
    gdtr.base = (uint32_t) gdt;
    gdtr.limit = sizeof(gdt);

    // use assembly function since c has no command to flush the gdt
    _flush_gdr( &gdtr);
}
