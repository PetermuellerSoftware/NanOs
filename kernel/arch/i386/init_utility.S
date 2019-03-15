.section .text
.section .text

/*
    c callable function to set the gdr
    needs a pointer to the gdtr struct
*/

.global _flush_gdr
.type _flush_gdr, @function
 
_flush_gdr:
    mov 4(%esp), %eax  /* Get the pointer to the GDT, passed as a parameter. */
    lgdt (%eax)        /* Load the new GDT pointer */

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    #mov $0x08, %ax
    jmp $0x08, $flush
flush:

    ret
.size _flush_gdr, . - _flush_gdr
