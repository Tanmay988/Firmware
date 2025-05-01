#include "L2P.h"
#include <stdio.h>

PhysicalBlockData_t l2p_table[TOTAL_SUPERBLOCKS][TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK];
uint32_t bad_physical_block[TOTAL_PHYSICALBLCKS] = {0};

void L2Pinit()
{
    uint32_t start = 0;
    printf("L2P: Initializing L2P Table...\n");
    for (int sb = 0; sb < TOTAL_SUPERBLOCKS; sb++)
    {
        int l = 0;
        int current = start;

        for (int bank = 0; bank < BANKS; bank++)
        {
            for (int k = current; k < current + (CHANNELS * PLANES); k++)
            {
                l2p_table[sb][l].block_id = k;
                l2p_table[sb][l].bank_no = k / (TOTAL_SUPERBLOCKS * CHANNELS * PLANES);
                l2p_table[sb][l].ch_no = (k % (CHANNELS * PLANES)) / PLANES;
                l2p_table[sb][l].plane_no = (k % 2 == 0) ? 0 : 1;
                l2p_table[sb][l].NBA = 0; // Optional: add actual formula if needed
                l++;
            }
            current += (TOTAL_SUPERBLOCKS * CHANNELS * PLANES);
        }

        start += (CHANNELS * PLANES);
    }
    printf("L2P: construction of L2P table is completed");
}

void l2p_printL2PTable()
{
    for (int sb = 0; sb < TOTAL_SUPERBLOCKS; sb++)
    {
        printf("\nL2P Superblock %d:\n", sb);
        for (int j = 0; j < TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK; j++)
        {
            printf("  Block ID: %4d, Bank: %2d, Channel: %2d, Plane: %d\n",
                   l2p_table[sb][j].block_id,
                   l2p_table[sb][j].bank_no,
                   l2p_table[sb][j].ch_no,
                   l2p_table[sb][j].plane_no);
        }
    }
}

void l2p_mark_bad_block(uint32_t block_id)
{
    if (block_id < TOTAL_PHYSICALBLCKS)
    {
        bad_physical_block[block_id] = 1;
        printf("L2P: Block %u marked as bad\n", block_id);
    }
    else
    {
        printf("L2P Error: Block ID %u out of range\n", block_id);
    }
}

// int main()
// {
//     printf("Initializing L2P Table...\n");
//     L2Pinit();
//     l2p_printL2PTable();

//     // l2p_mark_bad_block(5);
//     // l2p_mark_bad_block(100);

//     return 0;
// }