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