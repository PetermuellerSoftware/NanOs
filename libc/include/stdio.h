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

#ifndef _STDIO_H
#define _STDIO_H 1
 
#include <sys/cdefs.h>
#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)
 
#ifdef __cplusplus
extern "C" {
#endif
 
int printf(const char* __restrict, ...);
int sprintf(char* __restrict string, const char* __restrict, ...);
int snprintf(char* __restrict string, size_t n, const char* __restrict, ...);
int putchar(int);
int puts(const char*);

int vsnprintf(char* __restrict string, size_t n, const char* __restrict, va_list parameters);
int vsprintf(char* __restrict string, const char* __restrict, va_list parameters);
 
#ifdef __cplusplus
}
#endif
 
#endif