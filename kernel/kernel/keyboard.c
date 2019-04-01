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

#include "input_event_codes.h"

#include "kernel/baseIO.h"
#include "kernel/intr.h"
#include "kernel/log.h"
#include "arch/vga.h"


//#define LOG_CMD 1
//#define LOG_INTR_HANDLER 1
//#define LOG_KEYCODE 1
//#define LOG_SC2 1

#define KBD_STATE_INITIAL 0
#define KBD_STATE_E0 1
#define KBD_STATE_E1 2
#define KBD_STATE_F0 4
#define KBD_STATE_14 8

#define KBD_CMD_NODATA 0xff

#define KBD_QUEUE_LENGTH 16
unsigned char keyboard_cmd_buffer[ KBD_QUEUE_LENGTH ];
unsigned char keyboard_cmd_data_buffer[ KBD_QUEUE_LENGTH];
unsigned char buffer_pos;

int keyboard_state;
uint8_t resend_counter;
bool sending;

unsigned char keypressed_map[KEY_CNT >> 3];  // 8 bit per char

// max scan code 0x83
const unsigned char sc2_initial[132] = {
    KEY_RESERVED,
    KEY_F9,
    KEY_RESERVED,
    KEY_F5,
    KEY_F3,
    KEY_F1,
    KEY_F2,
    KEY_F12,
    KEY_RESERVED,
    KEY_F10,
    KEY_F8,
    KEY_F6,
    KEY_F4,
    KEY_TAB,
    KEY_APOSTROPHE,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_LEFTALT,
    KEY_LEFTSHIFT,
    KEY_RESERVED,
    KEY_LEFTCTRL,
    KEY_Q,
    KEY_1,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_Z,
    KEY_S,
    KEY_A,
    KEY_W,
    KEY_2,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_C,
    KEY_X,
    KEY_D,
    KEY_E,
    KEY_4,
    KEY_3,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_SPACE,
    KEY_V,
    KEY_F,
    KEY_T,
    KEY_R,
    KEY_5,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_N,
    KEY_B,
    KEY_H,
    KEY_G,
    KEY_Y,
    KEY_6,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_M,
    KEY_J,
    KEY_U,
    KEY_7,
    KEY_8,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_COMMA,
    KEY_K,
    KEY_I,
    KEY_O,
    KEY_0,
    KEY_9,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_DOT,
    KEY_SLASH,
    KEY_L,
    KEY_SEMICOLON,
    KEY_P, 
    KEY_MINUS,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_APOSTROPHE,
    KEY_RESERVED,
    KEY_LEFTMETA,
    KEY_EQUAL,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_CAPSLOCK,
    KEY_RIGHTSHIFT,
    KEY_ENTER,
    KEY_RIGHTMETA,
    KEY_RESERVED,
    KEY_BACKSLASH,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_BACKSPACE,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_KP1,
    KEY_RESERVED,
    KEY_KP4,
    KEY_KP7,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_KP0,
    KEY_KPDOT,
    KEY_KP2,
    KEY_KP5,
    KEY_KP6,
    KEY_KP8,
    KEY_ESC,
    KEY_NUMLOCK,
    KEY_F11,
    KEY_KPPLUS,
    KEY_KP3,
    KEY_KPMINUS,
    KEY_KPASTERISK,
    KEY_KP9,
    KEY_SCROLLLOCK,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_RESERVED,
    KEY_F7,
    
    } ;

void keyboard_send_command() {
    if (buffer_pos == 0 || sending) return;
    while ( (inb(0x64) & (0x1 << 1)) == 0); // wait for buffer to clear   
    while ( (inb(0x64) & (0x2 << 1)) == 0); // wait for reset mode 

    #ifdef LOG_CMD
    logf(LOG_TYPE_VERBOSE, "%s: send 0x%x to port 0x60\n", __func__, keyboard_cmd_buffer[0]);
    #endif
    outb(0x60, keyboard_cmd_buffer[0]);
    if(keyboard_cmd_data_buffer[0]!= KBD_CMD_NODATA ) {
        io_wait();            
        #ifdef LOG_CMD
        logf(LOG_TYPE_VERBOSE, "%s: send 0x%x to port 0x60\n", __func__, keyboard_cmd_data_buffer[0]);
        #endif
        outb(0x60, keyboard_cmd_data_buffer[0]);   
    }             
}

void keyboard_queue_command(unsigned char cmd, unsigned char data) {        
    if (buffer_pos < KBD_QUEUE_LENGTH) {
        #ifdef LOG_CMD
        logf(LOG_TYPE_VERBOSE, "%s: queue 0x%x with data 0x%x at pos %d\n", __func__, cmd, data, buffer_pos );
        #endif
        keyboard_cmd_buffer[buffer_pos] = cmd;
        keyboard_cmd_data_buffer[buffer_pos]=data;
        buffer_pos++;
        // trigger new send if queue was empty
        keyboard_send_command();        
    }  
        #ifdef LOG_CMD
        else      
        logf( LOG_TYPE_INFO, "%s:, queue was full", __func__);
        #endif
}
    
void keyboard_send_next_command() {
    memmove( keyboard_cmd_buffer, keyboard_cmd_buffer + 1, sizeof(keyboard_cmd_buffer) );
    memmove( keyboard_cmd_data_buffer, keyboard_cmd_data_buffer + 1, sizeof(keyboard_cmd_data_buffer) );
    keyboard_cmd_data_buffer[ KBD_QUEUE_LENGTH - 1] = KBD_CMD_NODATA;
    buffer_pos--;
    resend_counter = 0;
    keyboard_send_command();
}

inline void keyboard_setpressed (const unsigned char keycode, bool pressed) {
    const uint8_t pos = keycode >> 3;
    if (!keycode) 
        return;  // dont't do anything on keycode 0
    if (pressed) {        
        #ifdef LOG_KEYCODE
        logf( LOG_TYPE_VERBOSE, "keycode %x pressed\n", keycode );
        #endif
        keypressed_map[pos] |= 0x1 << (keycode && 0x7);
    }
    else {
        #ifdef LOG_KEYCODE
        logf( LOG_TYPE_VERBOSE, "keycode %x released\n", keycode );
        #endif
        keypressed_map[pos] &= ~(0x1 << (keycode && 0x7));        
    }
}


void keyboard_translate_sc2(const unsigned char scancode) {
    bool is_break = false;
    int keycode;

    // if we got a f0 before this is a break code
    if (keyboard_state == KBD_STATE_F0)
        is_break = true;
    
    #ifdef LOG_SC2
        logf( LOG_TYPE_VERBOSE, "%s: state=0x%x, code=%x\n", __func__, keyboard_state, scancode);            
    #endif
    
    if ( !(keyboard_state & (KBD_STATE_E0 | KBD_STATE_E1)  ) )
    {
        switch(scancode) {
        case 0xe0:            
            keyboard_state |= KBD_STATE_E0;
            break;
        case 0xe1:
            keyboard_state |= KBD_STATE_E1;
            break;
        case 0xf0:
            keyboard_state |= KBD_STATE_F0;
            break;
        default:         
            #ifdef LOG_SC2
            logf( LOG_TYPE_VERBOSE, "scancode 0x%x, break 0x%x\n", scancode, is_break);
            #endif
            // get keycode
            if (scancode <= 0x83 )
                keycode =  sc2_initial[scancode];
            else
                keycode = 0;
            keyboard_setpressed(keycode, !is_break);
            
            if (is_break)
                keyboard_state = KBD_STATE_INITIAL;
            break;
        }        
    }
}

void keyboard_intr_handler() {
    
    while (inb(0x64) & 0x1 ) {
        io_wait();
        uint8_t code = inb(0x60);
        #ifdef LOG_INTR_HANDLER
        logf(LOG_TYPE_VERBOSE, "%s: received 0x%x\n", __func__, code);
        #endif
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
            keyboard_translate_sc2(code);
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
    memset (keypressed_map, 0, sizeof(keypressed_map));    
    

    resend_counter = 0;
    keyboard_state = KBD_STATE_INITIAL;
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
}

