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

#define PIC_EOI 0x20

extern void _flush_idt( xdt_ptr_t* );

// space to store the idt in
uint8_t idt[256*8];

xdt_ptr_t idtr;

struct cpu_state {
    // Von Hand gesicherte Register
    uint32_t   eax;
    uint32_t   ebx;
    uint32_t   ecx;
    uint32_t   edx;
    uint32_t   esi;
    uint32_t   edi;
    uint32_t   ebp;

    uint32_t   intr;
    uint32_t   error;

    // Von der CPU gesichert
    uint32_t   eip;
    uint32_t   cs;
    uint32_t   eflags;
    uint32_t   esp;
    uint32_t   ss;
};

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

extern void* intr_stub_0;
extern void* intr_stub_1;
extern void* intr_stub_2;
extern void* intr_stub_3;
extern void* intr_stub_4;
extern void* intr_stub_5;
extern void* intr_stub_6;
extern void* intr_stub_7;
extern void* intr_stub_8;
extern void* intr_stub_9;
extern void* intr_stub_10;
extern void* intr_stub_11;
extern void* intr_stub_12;
extern void* intr_stub_13;
extern void* intr_stub_14;
extern void* intr_stub_15;
extern void* intr_stub_16;
extern void* intr_stub_17;
extern void* intr_stub_18;

extern void* intr_stub_32;
extern void* intr_stub_33;
extern void* intr_stub_34;
extern void* intr_stub_35;
extern void* intr_stub_36;
extern void* intr_stub_37;
extern void* intr_stub_38;
extern void* intr_stub_39;
extern void* intr_stub_40;
extern void* intr_stub_41;
extern void* intr_stub_42;
extern void* intr_stub_43;
extern void* intr_stub_44;
extern void* intr_stub_45;
extern void* intr_stub_46;
extern void* intr_stub_47;

extern void* intr_stub_48;


void kernel_init_idt()
{    
    memset (idt, 0, 8 * 256);
    
    log( LOG_TYPE_VERBOSE, "kernel_init_idt entry\n" );

    log( LOG_TYPE_DEBUG, "remapping PIC\n");
    PIC_remap( 0x20, 0x28);

    IDT_createEntry( idt + (0x00 << 3), &intr_stub_0);
    IDT_createEntry( idt + (0x01 << 3), &intr_stub_1);
    IDT_createEntry( idt + (0x02 << 3), &intr_stub_2);
    IDT_createEntry( idt + (0x03 << 3), &intr_stub_3);
    IDT_createEntry( idt + (0x04 << 3), &intr_stub_4);
    IDT_createEntry( idt + (0x05 << 3), &intr_stub_5);
    IDT_createEntry( idt + (0x06 << 3), &intr_stub_6);
    IDT_createEntry( idt + (0x07 << 3), &intr_stub_7);
    IDT_createEntry( idt + (0x08 << 3), &intr_stub_8);
    IDT_createEntry( idt + (0x09 << 3), &intr_stub_9);
    IDT_createEntry( idt + (0x0A << 3), &intr_stub_10);
    IDT_createEntry( idt + (0x0B << 3), &intr_stub_11);
    IDT_createEntry( idt + (0x0C << 3), &intr_stub_12);
    IDT_createEntry( idt + (0x0D << 3), &intr_stub_13);
    IDT_createEntry( idt + (0x0E << 3), &intr_stub_14);
    IDT_createEntry( idt + (0x0F << 3), &intr_stub_15);
    IDT_createEntry( idt + (0x10 << 3), &intr_stub_16);
    IDT_createEntry( idt + (0x11 << 3), &intr_stub_17);
    IDT_createEntry( idt + (0x12 << 3), &intr_stub_18);

    IDT_createEntry( idt + (0x20 << 3), &intr_stub_32);
    IDT_createEntry( idt + (0x21 << 3), &intr_stub_33);
    IDT_createEntry( idt + (0x22 << 3), &intr_stub_34);
    IDT_createEntry( idt + (0x23 << 3), &intr_stub_35);
    IDT_createEntry( idt + (0x24 << 3), &intr_stub_36);
    IDT_createEntry( idt + (0x25 << 3), &intr_stub_37);
    IDT_createEntry( idt + (0x26 << 3), &intr_stub_38);
    IDT_createEntry( idt + (0x27 << 3), &intr_stub_39);
    IDT_createEntry( idt + (0x28 << 3), &intr_stub_40);
    IDT_createEntry( idt + (0x29 << 3), &intr_stub_41);
    IDT_createEntry( idt + (0x2A << 3), &intr_stub_42);
    IDT_createEntry( idt + (0x2B << 3), &intr_stub_43);
    IDT_createEntry( idt + (0x2C << 3), &intr_stub_44);
    IDT_createEntry( idt + (0x2D << 3), &intr_stub_45);
    IDT_createEntry( idt + (0x2E << 3), &intr_stub_46);
    IDT_createEntry( idt + (0x2F << 3), &intr_stub_47);

    IDT_createEntry( idt + (0x30 << 3), &intr_stub_48);

    idtr.base = idt;
    idtr.limit = sizeof(idt);

    outb(PIC1_DATA, 0xFC ) ;
    outb(PIC2_DATA, 0xFF);

    log ( LOG_TYPE_VERBOSE, "IDT prepared\n");
    _flush_idt( &idtr);
    log ( LOG_TYPE_INFO, "IDT installed\n");
}

void handle_interrupt ( struct cpu_state* cpu) {
    if (cpu->intr <= 0x1F) {
        logf ( LOG_TYPE_CRITICAL, "CPU Exception %d\n", cpu->intr);
    } else {
//        logf ( LOG_TYPE_INFO, "Interrupt %d", cpu->intr);   
        
        if (cpu->intr>=0x28)
            outb(PIC2_COMMAND, PIC_EOI);         
        outb(PIC1_COMMAND, PIC_EOI);                
    }
}

