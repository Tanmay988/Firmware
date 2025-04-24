#include <stdio.h>
#include <stdint.h>
#include "FE.h"

typedef struct tag_PhysicalBlockData_t
{
    uint32_t block_id; // Logical Block ID
    uint32_t bank_no;
    uint32_t ch_no;
    uint32_t plane_no;
    uint32_t NBA;
} PhysicalBlockData_t;

PhysicalBlockData_t l2p_table[TOTAL_SUPERBLOCKS][TOTAL_PHYSICALBLCKS];
uint32_t bad_physical_block[TOTAL_PHYSICALBLCKS] = {0};

void L2Pinit()
{
    uint32_t start = 0;
    for (int i = 0; i < TOTAL_SUPERBLOCKS; i++)
    {
        // printf("\n L2P super block: %d\n", i);
        int l = 0;
        int current = start;
        for (int j = 0; j < BANKS; j++)
        {
            for (int k = current; k < current + (CHANNELS * PLANES); k++)
            {
                l2p_table[i][l].block_id = k;
                l2p_table[i][l].bank_no = k / (TOTAL_SUPERBLOCKS * CHANNELS * PLANES);
                l2p_table[i][l].ch_no = (k % (CHANNELS * PLANES)) / PLANES;
                l2p_table[i][l].plane_no = k % 2 == 0 ? 0 : 1;
                // l2p_table[i][l].NBA = // add formula
                l++;
            }
            current += (TOTAL_SUPERBLOCKS * CHANNELS * PLANES);
        }
        start += (CHANNELS * PLANES);
    }
}

void l2p_printL2PTable()
{
    for (int i = 0; i < TOTAL_SUPERBLOCKS; i++)
    {
        printf("\n L2P super block: %d\n", i);
        for (int j = 0; j < TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK; j++)
        {
            printf("Block ID: %d, Bank No: %d, Channel No: %d, Plane No: %d\n",
                   l2p_table[i][j].block_id,
                   l2p_table[i][j].bank_no,
                   l2p_table[i][j].ch_no,
                   l2p_table[i][j].plane_no);
        }
    }
}

// connect lbm retire blck function and l2p init function
void l2p_mark_bad_block(uint32_t block_id)
{
    bad_physical_block[block_id] = 1;
    printf("L2P: Block %d marked as bad\n", block_id);
}

int main()
{
    printf("L2P Table Initialization\n");
    L2Pinit();
    l2p_printL2PTable();

    l2p_mark_bad_block(5);
    l2p_mark_bad_block(100);

    return 0;
}
