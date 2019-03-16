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

.global c_isr
.extern c_isr

.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27

isr20:
    pushal
    push $0x0
    jmp isr_x    

isr21:
    pushal
    push $0x1
    jmp isr_x    

isr22:
    pushal
    push $0x2
    jmp isr_x    

isr23:
    pushal
    push $0x3
    jmp isr_x    

isr24:
    pushal
    push $0x4
    jmp isr_x    

isr25:
    pushal
    push $0x5
    jmp isr_x    

isr26:
    pushal
    push $0x6
    jmp isr_x    

isr27:
    pushal
    push $0x7
    jmp isr_x    

isr_x:
//    cld
    call c_isr
    pop %eax
    popal
    iret