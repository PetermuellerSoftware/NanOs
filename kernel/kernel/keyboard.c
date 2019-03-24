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

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "kernel/baseIO.h"
#include "kernel/intr.h"
#include "kernel/log.h"
#include "arch/vga.h"

#define KBD_STATE_INITIAL 0
#define KBD_STATE_E0 1
#define KBD_STATE_E0_F0 2
#define KBD_STATE_E1 3
#define KBD_STATE_E1_14 4
#define KBD_STATE_F0 5

#define KBD_CMD_NODATA 0xff

#define KBD_QUEUE_LENGTH 16
unsigned char keyboard_cmd_buffer[ KBD_QUEUE_LENGTH ];
unsigned char keyboard_cmd_data_buffer[ KBD_QUEUE_LENGTH];
unsigned char buffer_pos;

int keyboard_state;
uint8_t resend_counter;
bool sending;


void keyboard_send_command() {
    if (buffer_pos == 0 || sending) return;
    while (inb(0x64) & (0x1 << 1) == 0); // wait for buffer to clear   
    while (inb(0x64) & (0x2 << 1) == 0); // wait for reset mode 

    logf(LOG_TYPE_VERBOSE, "%s: send 0x%x to port 0x60\n", __func__, keyboard_cmd_buffer[0]);
    outb(0x60, keyboard_cmd_buffer[0]);
    if(keyboard_cmd_data_buffer[0]!= KBD_CMD_NODATA ) {
        io_wait();            
        logf(LOG_TYPE_VERBOSE, "%s: send 0x%x to port 0x60\n", __func__, keyboard_cmd_data_buffer[0]);
        outb(0x60, keyboard_cmd_data_buffer[0]);   
    }             
}

void keyboard_queue_command(unsigned char cmd, unsigned char data) {        
    if (buffer_pos < KBD_QUEUE_LENGTH) {
        logf(LOG_TYPE_VERBOSE, "%s: queue 0x%x with data 0x%x at pos %d\n", __func__, cmd, data, buffer_pos );
        keyboard_cmd_buffer[buffer_pos] = cmd;
        keyboard_cmd_data_buffer[buffer_pos]=data;
        buffer_pos++;
        // trigger new send if queue was empty
        keyboard_send_command();        
    } else
        logf( LOG_TYPE_INFO, "%s:, queue was full", __func__);
    
}
    
void keyboard_send_next_command() {
    memmove( keyboard_cmd_buffer, keyboard_cmd_buffer + 1, sizeof(keyboard_cmd_buffer) );
    memmove( keyboard_cmd_data_buffer, keyboard_cmd_data_buffer + 1, sizeof(keyboard_cmd_data_buffer) );
    keyboard_cmd_data_buffer[ KBD_QUEUE_LENGTH - 1] = KBD_CMD_NODATA;
    buffer_pos--;
    resend_counter = 0;
    keyboard_send_command();
}

void keyboard_translate_sc2(unsigned char scancode) {
    switch (keyboard_state) {
    case KBD_STATE_INITIAL:
        switch(scancode) {
        case 0xe0:            
            keyboard_state = KBD_STATE_E0;
            break;
        case 0xe1:
            keyboard_state = KBD_STATE_E1;
            break;
        case 0xf0:
            keyboard_state = KBD_STATE_F0;
            break;
        case 0x3A:
            terminal_putchar("M");
            break;
        default:
            break;
        }        
    }
}

void keyboard_intr_handler() {
//    log ( LOG_TYPE_INFO, "kbd\n");
    
    while (inb(0x64) & 0x1 ) {
        io_wait();
        uint8_t code = inb(0x60);
        logf(LOG_TYPE_VERBOSE, "%s: received 0x%x\n", __func__, code);
        switch (code) {
        case 0xFA: // send next command            
            sending = false;
            keyboard_send_next_command();
            break;
        case 0xFE: // resend command
            if (++resend_counter > 3)
                keyboard_send_next_command();
            else 
                keyboard_send_command();
            break;            
        default:
        break;
        }
    }
}

void kbc_disable_translation() {
    // read out controller configuration byte
    outb( 0x64, 0x20);
    // wait until it arrives
    while (!inb(0x64) && 1);
    const unsigned char ccb = inb(0x60);

    // write to controller configuration byte    
    outb( 0x64, 0x60);
    // wait until buffer is free again
    while (!inb(0x64) && 2);
    // send back with bit 6 cleared
    outb( 0x60, ccb & 0xbf );
}

void keyboard_init() {
    // register the interrupt handler for the keyboard
    register_intr_handler( INTR_BASE + 0x01, &keyboard_intr_handler);
        
    // clear remaining buffer
    while ( inb(0x64) & 0x1 ) {
        io_wait();
        inb(0x60);
    }

    // clear command buffer
    memset (keyboard_cmd_buffer, 0, sizeof(keyboard_cmd_buffer));
    memset (keyboard_cmd_data_buffer, KBD_CMD_NODATA, sizeof(keyboard_cmd_buffer));
    
    resend_counter = 0;
    keyboard_state = 0;
    sending = false;

    kbc_disable_translation();

    // reinit keyboard
    keyboard_queue_command(0xf5, KBD_CMD_NODATA);

    // ask for keyboard type
    keyboard_queue_command(0xf2, KBD_CMD_NODATA);

    // set scancode set 2
    keyboard_queue_command(0xf0, KBD_CMD_NODATA);
    keyboard_queue_command(0x02, KBD_CMD_NODATA);

    // start scanning
    keyboard_queue_command(0xF4, KBD_CMD_NODATA);            

    // check scancode set 2
    keyboard_queue_command(0xf0, KBD_CMD_NODATA);
    keyboard_queue_command(0x00, KBD_CMD_NODATA);

    //logf( LOG_TYPE_INFO, "keyboard scancode is %d\n", scan_code);

}

