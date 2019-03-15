#include <kernel/serial.h>
#include <kernel/baseIO.h>


void serial_write(const char* data, size_t size) {
    char LSR = 0;
    
    for (size_t i=0; i < size; i++){
        // wait to have free space in fifo
        do {
            // read LSR
            LSR = inb(SERIAL_PORT_1_BASE + SERIAL_REGISTER_LSR);
        } while ( LSR & SERIAL_LSR_FIFO_TX_OK );
        // send daa
        outb(SERIAL_PORT_1_BASE + SERIAL_REGISTER_TX, data[i]);
    }
}


