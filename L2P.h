#ifndef L2P_H
#define L2P_H

#include "FE.h"
#include <stdint.h>
// Represents a physical block entry in the L2P table
typedef struct tag_PhysicalBlockData_t
{
    uint32_t block_id; // Logical Block ID
    uint32_t bank_no;  // Bank number
    uint32_t ch_no;    // Channel number
    uint32_t plane_no; // Plane number
    uint32_t NBA;      // Numerical Block Address (if used)
} PhysicalBlockData_t;

// Global L2P table and bad block list
extern PhysicalBlockData_t l2p_table[TOTAL_SUPERBLOCKS][TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK];
extern uint32_t bad_physical_block[TOTAL_PHYSICALBLCKS];

// Initializes the L2P table with calculated mappings
void L2Pinit(void);

// Prints the L2P table to stdout
void l2p_printL2PTable(void);

// Marks a physical block as bad
void l2p_mark_bad_block(uint32_t block_id);

#endif // L2P_H
