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


