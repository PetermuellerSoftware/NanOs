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

#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool __vprintf(const char* data, size_t length) {
    const unsigned char* bytes = (const unsigned char*) data;
    for (size_t i = 0; i < length; i++)
        if (putchar(bytes[i]) == EOF)
            return false;
    return true;
}


int vprintf(const char* restrict format, va_list parameters) {
 
    int written = 0;
    while (*format != '\0') {
        size_t maxrem = INT_MAX - written;

        if (format[0] != '%' || format[1] == '%') {
            if (format[0] == '%')
                format++;
            size_t amount = 1;

            while (format[amount] && format[amount] != '%')

                amount++;

            if (maxrem < amount) {

                // TODO: Set errno to EOVERFLOW.

                return -1;

            }

            if (!print(format, amount))

                return -1;

            format += amount;

            written += amount;

            continue;

        }

 

        const char* format_begun_at = format++;

 

        if (*format == 'c') {

            format++;

            char c = (char) va_arg(parameters, int /* char promotes to int */);

            if (!maxrem) {

                // TODO: Set errno to EOVERFLOW.

                return -1;

            }

            if (!print(&c, sizeof(c)))

                return -1;

            written++;

        } else if (*format == 's') {

            format++;

            const char* str = va_arg(parameters, const char*);

            size_t len = strlen(str);

            if (maxrem < len) {

                // TODO: Set errno to EOVERFLOW.

                return -1;

            }

            if (!print(str, len))

                return -1;

            written += len;

        } else {

            format = format_begun_at;

            size_t len = strlen(format);

            if (maxrem < len) {

                // TODO: Set errno to EOVERFLOW.

                return -1;

            }

            if (!print(format, len))

                return -1;

            written += len;

            format += len;

        }

    }


    return written;

}