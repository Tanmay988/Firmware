#include <stdio.h>
#include <stdint.h>

#include "LUT.h"

MCA_t LUT_Table[TOTAL_LCAs] = {0}; // LUT table for LCA to MCA mapping

void LUTinit()
{
    for (int i = 0; i < TOTAL_LCAs; i++)
    {
        LUT_Table[i].MBA = UNMAPPED_VALUE;
        LUT_Table[i].mco.wl = UNMAPPED_VALUE;
        LUT_Table[i].mco.str = UNMAPPED_VALUE;
        LUT_Table[i].mco.bank = UNMAPPED_VALUE;
        LUT_Table[i].mco.page = UNMAPPED_VALUE;
        LUT_Table[i].mco.channel = UNMAPPED_VALUE;
        LUT_Table[i].mco.plane = UNMAPPED_VALUE;
        LUT_Table[i].mco.cluster_offset = UNMAPPED_VALUE;
    }
}

void lut_update_request(uint32_t LCA, MCA_t *mca)
{
    LUT_Table[LCA].MBA = mca->MBA;
    LUT_Table[LCA].mco.wl = mca->mco.wl;
    LUT_Table[LCA].mco.str = mca->mco.str;
    LUT_Table[LCA].mco.bank = mca->mco.bank;
    LUT_Table[LCA].mco.page = mca->mco.page;
    LUT_Table[LCA].mco.channel = mca->mco.channel;
    LUT_Table[LCA].mco.plane = mca->mco.plane;
    LUT_Table[LCA].mco.cluster_offset = mca->mco.cluster_offset;

    printf("\nLUT: LCA %u mapping updated to MCA.\n", LCA);
}

uint32_t lut_read_request(uint32_t LCA)
{
    MCA_t mca = LUT_Table[LCA];

    if (mca.MBA == UNMAPPED_VALUE)
    {
        printf("LUT: Read failed, LCA %u is unmapped\n", LCA);
        return 0;
    }

    printf("LUT: Read request for LCA %u mapped to MCA\n", LCA);
    printf("LUT: MBA: %u, WL: %u, STR: %u, BANK: %u, PAGE: %u, CHANNEL: %u, PLANE: %u\n",
           mca.MBA,
           mca.mco.wl,
           mca.mco.str,
           mca.mco.bank,
           mca.mco.page,
           mca.mco.channel,
           mca.mco.plane);
    printf("LUT: CLUSTER OFFSET: %u\n", mca.mco.cluster_offset);
    return fil_read_request(mca);
}
