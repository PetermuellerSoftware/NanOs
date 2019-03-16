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
#include <stdlib.h>
#include <string.h>

#include <kernel/log.h>
#include <kernel/serial.h>
#include <kernel/tty.h>


//	extern "C" 
void kernel_main(void) 
{    
    // give the fist sign of live to the user
    log (LOG_TYPE_INFO, "NanOs is starting!\n\n");

    
    
    
    while(1)
        asm volatile ("hlt");
    
    log (LOG_TYPE_ERROR, "Never should come to here, kernel leaving main");
}

