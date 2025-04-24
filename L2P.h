#ifndef L2P_H
#define L2P_H

#include <stdint.h>
#include "FE.h" // Assumes FE.h defines macros like TOTAL_SUPERBLOCKS, TOTAL_PHYSICALBLCKS, etc.

// Structure representing a physical block entry in the L2P table
typedef struct tag_PhysicalBlockData_t
{
    uint32_t block_id;   // Logical Block ID
    uint32_t bank_no;    // Bank number
    uint32_t ch_no;      // Channel number
    uint32_t plane_no;   // Plane number
    uint32_t NBA;        // Numerical Block Address
} PhysicalBlockData_t;

// Global L2P table and bad block table
extern PhysicalBlockData_t l2p_table[TOTAL_SUPERBLOCKS][TOTAL_PHYSICALBLCKS];
extern uint32_t bad_physical_block[TOTAL_PHYSICALBLCKS];

// Function declarations
void L2Pinit(void);
void l2p_printL2PTable(void);
void l2p_mark_bad_block(uint32_t block_id);

#endif // L2P_H
