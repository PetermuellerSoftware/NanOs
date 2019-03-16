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


#include <kernel/baseIO.h>
#include <kernel/GDT.h>
#include <kernel/log.h>

#include <stdio.h>
#include <string.h>

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


// define PIC ports

#define PIC1 0x20
#define PIC2 0xA0
#define PIC1_COMMAND PIC1 
#define PIC1_DATA PIC1+1
#define PIC2_COMMAND PIC2 
#define PIC2_DATA PIC2+1

extern void _flush_idt( xdt_ptr_t* );


// space to store the idt in
uint8_t idt[256*8];

xdt_ptr_t idtr;

struct IDTDescr {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
};

typedef struct IDTDescr IDTDescr_t;
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

void IDT_createEntry( uint8_t* idt_entry, void* interrupt_service_routine)
{   
    IDTDescr_t *entry = (IDTDescr_t*) idt_entry;
    
    entry->selector = 0x08;
    entry->type_attr = 0x8E; //present, ring 0, 32-bit interrupt gate
    entry->offset_1 = (int) interrupt_service_routine & 0xffff;
    entry->offset_2 = ( (int) interrupt_service_routine >> 16);
    entry->zero = 0x00;
}

extern void* isr20;
extern void* isr21;
extern void* isr22;
extern void* isr23;
extern void* isr24;
extern void* isr25;
extern void* isr26;
extern void* isr27;

void kernel_init_idt()
{    
    memset (idt, 0, 8 * 256);
    
    log( LOG_TYPE_VERBOSE, "kernel_init_idt entry\n" );

    log( LOG_TYPE_DEBUG, "remapping PIC\n");
    PIC_remap( 0x20, 0x28);

    for (int i=0; i < 0x20; i++)
        IDT_createEntry( idt + (i << 3), &isr20);        
 
    IDT_createEntry( idt + (0x20 << 3), &isr20);
    IDT_createEntry( idt + (0x21 << 3), &isr21);
    IDT_createEntry( idt + (0x22 << 3), &isr22);
    IDT_createEntry( idt + (0x23 << 3), &isr23);
    IDT_createEntry( idt + (0x24 << 3), &isr24);
    IDT_createEntry( idt + (0x25 << 3), &isr25);
    IDT_createEntry( idt + (0x26 << 3), &isr22);
    IDT_createEntry( idt + (0x27 << 3), &isr27);
    
    idtr.base = idt;
    idtr.limit = sizeof(idt);

// nothing but keyboard right now
    outb(PIC1_DATA, !0x02) ;
    outb(PIC2_DATA, !0x00);

    log ( LOG_TYPE_VERBOSE, "IDT prepared\n");
    _flush_idt( &idtr);
    log ( LOG_TYPE_INFO, "IDT installed\n");
}

void c_isr( uint8_t intr ) {
    logf( LOG_TYPE_VERBOSE, "interrupt %d raised", intr);
        
    if (intr>=8)
        outb(PIC2_COMMAND, 20);
    outb(PIC1_COMMAND, 20);
}


