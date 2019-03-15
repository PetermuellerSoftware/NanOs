
#include <stdint.h>

#define MULTIBOOT_FLAG_MEM 1<<0
#define MULTIBOOT_FLAG_BOOT_DEVICE 1<<1
#define MULTIBOOT_FLAG_CMD_LINE 1<<2
#define MULTIBOOT_FLAG_MODS 1<<3
#define MULTIBOOT_FLAG_4 1<<4
#define MULTIBOOT_FLAG_5 1<<5
#define MULTIBOOT_FLAG_MMAP 1<<6

struct multiboot_information_structure {
    uint32_t flags;
    
    uint32_t mem_lower; // valid with flag mem_info
    uint32_t mem_upper;
    
    uint32_t boot_device;
    
    char* cmd_lime;
    
    uint32_t mods_count;
    uint32_t mods_addr;
    
    uint32_t tabsize;
    uint32_t strsize;
    uint32_t addr;
    uint32_t reserved;
    
    uint32_t mmap_length;
    uint32_t mmap_address;
    
}__attribute__((packed));


typedef struct multiboot_information_structure multiboot_information_t;

extern multiboot_information_t* multiboot_information;