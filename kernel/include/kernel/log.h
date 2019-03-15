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

#ifndef KERNEL_LOG_H
#define KERNEL_LOG_H 1

enum LOG_TYPE {
    LOG_TYPE_CRITICAL = 1,
    LOG_TYPE_ERROR = 2,
    LOG_TYPE_WARNING = 3,
    LOG_TYPE_INFO = 4,
    LOG_TYPE_DEBUG = 5,    
    LOG_TYPE_VERBOSE = 6    
};

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_TYPE_DEBUG
#endif

void log(int level, const char* msg);
void logf(int level, const char* format, ...);

#endif