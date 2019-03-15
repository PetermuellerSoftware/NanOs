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

#include "kernel/GDT.h"
#include "kernel/baseIO.h"
//#include "vga.h"

//#include "MemoryController.h"

// space to store the gdt in
uint8_t gdt[6*8];
// space to store the idt in
uint8_t idt[256*8];

xdt_ptr_t gdtr;
xdt_ptr_t idtr;

void _flush_gdr( xdt_ptr_t* ); 
void _flush_idr( xdt_ptr_t* );

//extern vga vga;

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
    
    #define GDT_MAX_SIZE 6
    gdt_source source[ GDT_MAX_SIZE ];
    
    source[0].base = 0;
    source[0].limit = 0;
    source[0].flags= 0;
    source[0].access = 0;
    source[0].ring = 0;
    
// kernel space
    source[1].base=0;
    source[1].limit=0xffffffff;
    source[1].flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE;
    source[1].access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT;
    source[1].ring=0;
    
    source[2].base=0;
    source[2].limit=0xffffffff;
    source[2].flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE;
    source[2].access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT;
    source[2].ring=0;

 // user space 
   source[3] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_EXECUTABLE | GDT_ACCESS_RW | GDT_ACCESS_PRESENT, 
        .ring=3 }; // Selector 0x18 user code
    source[4] = (gdt_source) {
        .base=0, 
        .limit=0xffffffff, 
        .flags= GDT_FLAG_GRANULARITY | GDT_FLAG_SIZE, 
        .access= GDT_ACCESS_CODE_DATA | GDT_ACCESS_RW | GDT_ACCESS_PRESENT,
        .ring=3 }; // Selector 0x20 user data
       
    GDT_createEntry( gdt, source[0] );
    GDT_createEntry( gdt+8, source[1] );
    GDT_createEntry( gdt+16, source[2] );
    GDT_createEntry( gdt+24, source[3] );
    GDT_createEntry( gdt+32, source[4] );
    
    
    gdtr.base = (uint32_t) gdt;
    gdtr.limit = sizeof(gdt);

    // use assembly function since c has no command to flush the gdt
    _flush_gdr( &gdtr);
}


void IDT_createEntry( uint8_t* idt_entry, uint32_t base, uint32_t limit)
{

}

void kernel_init_idt()
{
    
}

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/
void PIC_remap(int offset1, int offset2)
{
	unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);
}

void kernel_init()
{
    // initialize the gdt
    kernel_init_gdt();

    // remap hw interupts from 0x20 to 0x2F
    PIC_remap(0x20,0x28);

    // initialize the idt
    kernel_init_idt();
    
    //initializePhysicalBlocks();
}