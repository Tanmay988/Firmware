#include <stdio.h>
#include <stdint.h>
#include "WC.h"

extern LogicalBlock_t input_list;
static uint32_t block_assigned = 0;

void WCinit(MCA_t *mca)
{
    if (!mca)
    {
        printf("WC Error: NULL pointer passed to WCinit\n");
        return;
    }

    mca->MBA = TOTAL_SUPERBLOCKS + 1;
    mca->mco.wl = 0;
    mca->mco.str = 0;
    mca->mco.bank = 0;
    mca->mco.page = 0;
    mca->mco.channel = 0;
    mca->mco.plane = 0;
    mca->mco.cluster_offset = 0;

    printf("WC: MCA structure initialized to default values.\n");
}

uint32_t wc_write_request(uint32_t lca)
{
    printf("WC: Write request for LCA %u\n", lca);
    static MCA_t mca = {0}; // MCA context for current write

    // Allocate block if not assigned
    if (block_assigned == 0)
    {
        input_list = lbm_allocate_block();
        block_assigned = 1;
        fil_erase_block(input_list);
        mca.MBA = input_list.mba;
        mca.mco.cluster_offset = 0;
    }

    mca.mco.wl = input_list.wl;
    mca.mco.str = input_list.str;
    mca.mco.bank = l2p_table[mca.MBA][input_list.physical_block_id].bank_no;
    mca.mco.channel = l2p_table[mca.MBA][input_list.physical_block_id].ch_no;
    mca.mco.plane = l2p_table[mca.MBA][input_list.physical_block_id].plane_no;
    mca.mco.page = 0;

    // Write via FIL and update LUT
    if (fil_write_request(lca, &mca))
    {
        lut_update_request(lca, &mca);
    }
    else
    {
        printf("WC: Write request failed for LCA %u\n", lca);
        return 0;
    }

    if (mca.mco.cluster_offset >= 3)
    {
     
        mca.mco.cluster_offset = 0;
        input_list.wl++;

        if (input_list.wl >= WORDLINES)
        {
            input_list.wl = 0;
            input_list.str++;
        }

        if (input_list.str >= STRINGS)
        {
            input_list.vccount++;
            input_list.physical_block_id++;
            input_list.str = 0;
            input_list.wl = 0;
        }
    }
    else
    {
        mca.mco.cluster_offset++; 
    }

    if (input_list.physical_block_id >= TOTAL_PHYSICALbLOCK_PER_SUPERBLOCK)
    {
        block_assigned = 0;
        lbm_move_input_to_active(input_list);
    }

    return 1;
}
