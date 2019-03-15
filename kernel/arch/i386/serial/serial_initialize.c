#include <kernel/serial.h>
#include <kernel/baseIO.h>

void serial_initialize ( int baud) {
    const char LCR = SERIAL_LCR_8BIT | SERIAL_LCR_PARITY_NONE;

    // open DLAB access
    outb( SERIAL_PORT_1_BASE + SERIAL_REGISTER_LCR, SERIAL_LCR_DLAB );
    // set baud rate
    outb( SERIAL_PORT_1_BASE + SERIAL_REGISTER_DLL, baud & 0xff );
    outb( SERIAL_PORT_1_BASE + SERIAL_REGISTER_DLH, (baud>>8) & 0xff );
    // close DLAB access    
    io_wait();
    outb( SERIAL_PORT_1_BASE + SERIAL_REGISTER_LCR, LCR );    
    io_wait();
    
    outb (SERIAL_PORT_1_BASE + SERIAL_REGISTER_FCR, 0x27 );
    io_wait();
    outb (SERIAL_PORT_1_BASE + SERIAL_REGISTER_FCR, 0x21 );
}