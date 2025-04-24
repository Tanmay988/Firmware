// FE_H
#ifndef FE_H
#define FE_H


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
#define TOTAL_Device_Capacity 6144                        // in MB (6GB)
#define TOTAL_SUPERBLOCKS (TOTAL_Device_Capacity / SUPERBLOCK_SIZE)  // 16 super blocks
#define TOTAL_PHYSICALBLCKS (TOTAL_Device_Capacity / PHYSICALBLOCK_SIZE) // 1024 physical blocks
#define TOTAL_PHYSICAL_BLOCKS_PER_SUPER_BLOCK (384 / 6)
#define TOTAL_LCAs ((TOTAL_Device_Capacity * 1024) / 4)   // each LCA = 4KB
#define TOTAL_LBAs (TOTAL_LCAs * 8)                       // 1 LCA = 8 LBAs
#define TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK (SUPERBLOCK_SIZE/PHYSICALBLOCK_SIZE) 
#endif // CONFIG_H
