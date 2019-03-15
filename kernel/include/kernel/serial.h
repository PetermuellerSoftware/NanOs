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

#ifndef SERIAL_H
#define SERIAL_H 1

#include <stddef.h>

#define SERIAL_PORT_1_BASE 0x3f8
#define SERIAL_PORT_2_BASE 0x2f8
#define SERIAL_PORT_3_BASE 0x3e8
#define SERIAL_PORT_4_BASE 0x2e8

#define SERIAL_REGISTER_TX 0x0
#define SERIAL_REGISTER_RX 0x0
#define SERIAL_REGISTER_DLL 0x0
#define SERIAL_REGISTER_DLH 0x1
#define SERIAL_REGISTER_IER 0x1
#define SERIAL_REGISTER_IIR 0x2
#define SERIAL_REGISTER_FCR 0x2
#define SERIAL_REGISTER_LCR 0x3
#define SERIAL_REGISTER_MCR 0x4
#define SERIAL_REGISTER_LSR 0x5
#define SERIAL_REGISTER_MSR 0x6
#define SERIAL_REGISTER_SR 0x7

#define SERIAL_LCR_5BIT 0x0
#define SERIAL_LCR_6BIT 0x1
#define SERIAL_LCR_7BIT 0x2
#define SERIAL_LCR_8BIT 0x3
#define SERIAL_LCR_2STOPP 0x1<<2
#define SERIAL_LCR_PARITY_NONE  0x0<<3
#define SERIAL_LCR_PARITY_ODD   0x1<<3
#define SERIAL_LCR_PARITY_EVEN  0x3<<3
#define SERIAL_LCR_PARITY_MARK  0x5<<3
#define SERIAL_LCR_PARITY_SPACE 0x7<<3
#define SERIAL_LCR_BREAK 0x1<<6
#define SERIAL_LCR_DLAB 0x1<<7

#define SERIAL_LSR_DATA_READY 0x0<0
#define SERIAL_LSR_OVERRUN_ERROR 0x0<1
#define SERIAL_LSR_PARITY_ERROR 0x0<2
#define SERIAL_LSR_FRAMING_ERROR 0x0<3
#define SERIAL_LSR_BREAK_INTERRUPT 0x0<4
#define SERIAL_LSR_FIFO_TX_OK 0x0<5
#define SERIAL_LSR_FIFO_TX_EMPTY 0x0<6
#define SERIAL_LSR_FIFO_RX_ERROR 0x0<7


enum SERIAL_BAUD_RATE {
    SERIAL_BAUD_RATE_115200 = 1,
    SERIAL_BAUD_RATE_57600 = 2,
    SERIAL_BAUD_RATE_38400 = 3,
    SERIAL_BAUD_RATE_19200= 6,
    SERIAL_BAUD_RATE_9600 = 12,    
};

void serial_initialize ( int );
void serial_write(const char * data, size_t size);
void serial_writestring ( const char * string);

#endif