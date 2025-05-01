#ifndef FE_H
#define FE_H

#include <stdint.h>

// NAND Configuration
#define WORDLINES 96
#define STRINGS 4
#define PLANES 2
#define BANKS 4
#define CHANNELS 8

// Page size in KB
#define PAGE_SIZE 16

// Derived sizes in MB
#define PHYSICALBLOCK_SIZE ((PAGE_SIZE * WORDLINES * STRINGS) / 1024)
#define SUPERBLOCK_SIZE (PHYSICALBLOCK_SIZE * CHANNELS * BANKS * PLANES)

// Functional requirements
#define TOTAL_Device_Capacity 6144                                       // in MB (6GB)
#define TOTAL_SUPERBLOCKS (TOTAL_Device_Capacity / SUPERBLOCK_SIZE)      // 16 super blocks
#define TOTAL_PHYSICALBLCKS (TOTAL_Device_Capacity / PHYSICALBLOCK_SIZE) // 1024 physical blocks
#define TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK (SUPERBLOCK_SIZE / PHYSICALBLOCK_SIZE)

#define TOTAL_LCAs ((TOTAL_Device_Capacity * 1024) / 4) // each LCA = 4KB
#define TOTAL_LBAs (TOTAL_LCAs * 8)                     // 1 LCA = 8 LBAs

void fe_boot_modules();
uint32_t fe_lba_lca(uint32_t lba);
void fe_write_request(uint32_t start_LBA, uint32_t no_LBA);
void fe_read_request(uint32_t start_LBA, uint32_t no_LBA);

#endif // FE_H
