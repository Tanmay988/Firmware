#include <stdio.h>
#include <stdint.h>

#include "FE.h"
#include "WC.h"

#define UNMAPPED_VALUE 0xffffffff

MCA_t LUT_Table[TOTAL_LCAs] = {0}; // LUT table for LCA to MCA mapping

void LUTinit()
{
    for (int i = 0; i < TOTAL_LCAs; i++)
    {
        LUT_Table[i].MBA = UNMAPPED_VALUE; // Initialize all entries to UNMAPPED_VALUE
        LUT_Table[i].mco.wl = UNMAPPED_VALUE;
        LUT_Table[i].mco.str = UNMAPPED_VALUE;
        LUT_Table[i].mco.bank = UNMAPPED_VALUE;
        LUT_Table[i].mco.page = UNMAPPED_VALUE;
        LUT_Table[i].mco.channel = UNMAPPED_VALUE;
        LUT_Table[i].mco.plane = UNMAPPED_VALUE;
        LUT_Table[i].mco.cluster_offset = UNMAPPED_VALUE;
    }
}

void lut_update_request(uint32_t LCA, MCA_t mca)
{
    LUT_Table[LCA] = mca;
    printf("\n LUT: LCA to MCA Mapping is updated.");
}

uint8_t lut_read_request(uint32_t LCA)
{
    MCA_t mca = LUT_Table[LCA];

    if (mca.MBA == UNMAPPED_VALUE)
    {
        printf("LUT: Read failed, LCA %u is unmapped\n", LCA);
        return 0; // Mapping not found
    }

    // Mapping exists, forward read request
    printf("LUT: Read request for LCA %u mapped to MCA\n", LCA);
    return fil_read_request(mca); // Request forwarded successfully
}
