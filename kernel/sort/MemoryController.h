#include <stdint.h>

/*
 *  Physical memory management 
 * 
 */
extern "C" void initializePhysicalBlocks();


uint32_t get_physical_block ();
void free_physical_block ();
