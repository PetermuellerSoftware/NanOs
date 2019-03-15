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

#ifndef STRING_H
#define STRING_H 1

#include <sys/cdefs.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif

int memcmp (const void * , const void * , size_t );
void* memcpy(void* __restrict, const void* __restrict, size_t);
void * memmove ( void * , const void * , size_t );
void * memset ( void * , int value, size_t );

size_t strlen ( const char * str );

#ifdef __cplusplus
}
#endif

#endif