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


#include <stdio.h>

#include "kernel/baseIO.h"
#include "kernel/GDT.h"
#include "kernel/log.h"
#include "kernel/serial.h"
#include "kernel/tty.h"

//#include "MemoryController.h"

extern void kernel_init_gdt();
extern void kernel_init_idt();

void kernel_init()
{
    terminal_initialize();
        
    // serial not ready yet, so dont use log
    printf ("terminal initialized\n"); 
    
    serial_initialize( SERIAL_BAUD_RATE_57600);

    log ( LOG_TYPE_VERBOSE, "serial port initialized\n" );
    
    // initialize the gdt
    kernel_init_gdt();

    log ( LOG_TYPE_INFO, "global descriptor table installed\n" );

    // initialize the idt
    kernel_init_idt();
}