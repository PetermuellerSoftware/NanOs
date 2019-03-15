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

#include <stdint.h>
#include <stdbool.h>

/*  Segment structure
 * 
 *  31 30 29 28 27 26 25 24 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
 *  +- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --+-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -+
 *  | base 24-31           |  Flags    |limit16-19 | Access byte           | base 16-23           |
 *  +----------------------------------------------+----------------------------------------------+
 *  |         base 0 -15                           |            limit -15                         |
 *  +----------------------------------------------+----------------------------------------------+
 * 
 *  Flags
 *  20 - not used
 *  21 - L  Large only x64
 *  22 - Sz Size  (0=16 bit, 1=32 bit)
 *  23 - G  granularity (0=1b-1Mb, 1=4kb-4GB)
 * 
 *  Access byte
 *  08 - Ac Accessed, set from CPU when used in segment register
 *  09 - RW ReadWrite (Readable for Code Segments, writeable for data segments)
 *  10 - DC Dircetion bit ( 0= grows up, 1= grows down)
 *  11 - Ex Exectuable (0= Data, 1= Code)
 *  12 - S  Descritpor Type (1= code/data, 0= System)
 *  13 - Privl
 *  14 -    Privilege (ring level)
 *  15 - P  Present (1=valied, 0 not valid)
 */

#define GDT_FLAG_LARGE 1 << 5
#define GDT_FLAG_SIZE 1 << 6
#define GDT_FLAG_GRANULARITY 1 << 7

#define GDT_ACCESS_RW 1 << 1
#define GDT_ACCESS_DIRECTION_DOWN 1 << 2
#define GDT_ACCESS_EXECUTABLE 1 << 3
#define GDT_ACCESS_CODE_DATA 1 << 4
#define GDT_ACCESS_PRESENT 1 << 7


/* keeps information about the gdt_entry to build
 */
struct gdt_source_struct
{
    uint32_t base;
    uint32_t limit;  // dont use more than 20 bit
    
    uint8_t flags;    
    uint8_t access;
    uint8_t ring;
};

typedef struct gdt_source_struct gdt_source; 

struct xdt_ptr_struct
{
    uint16_t limit;
    uint32_t base;
}__attribute__((packed));
 

typedef struct xdt_ptr_struct xdt_ptr_t;

