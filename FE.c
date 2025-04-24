#include <stdio.h>
#include <stdint.h>

#include "FE.h"

void fe_boot_modules()
{
    L2Pinit();
    LBMinit();
    WCinit();
    LUTinit();
    FILinit();
}
void fe_write_request(uint32_t start_LBA, uint32_t no_LBA)
{
    if (start_LBA >= TOTAL_LBAs || start_LBA + no_LBA > TOTAL_LBAs || start_LBA < 0 || no_LBA <= 0)
    {
        printf("FE: Invalid LBA Range (Start: %u, Count: %u)\n", start_LBA, no_LBA);
        return;
    }

    printf("FE: Write request received: Start_LBA = %u, Count = %u\n", start_LBA, no_LBA);

    uint32_t end_LBA = start_LBA + no_LBA;
    uint32_t current_LBA = start_LBA;
    uint32_t no_of_LCA = fe_lba_lca(end_LBA - current_LBA);

    while (current_LBA < end_LBA)
    {
        uint32_t lca = current_LBA / 8;

        printf("FE: Converted LBA %u to LCA %u\n", current_LBA, lca);

        // Align LBA to next 16KB boundary (32 sectors)
        current_LBA += 32;

        for (uint32_t i = 0; i < 4; i++)
        {
            // Send to LUT for each LCA involved in a 16KB block
            if (!wc_write_request(lca++))
            {
                printf("FE: LUT write failed for LCA = %u\n", lca);
            }
        }
    }

    printf("FE: Write request completed successfully.\n");
}

void fe_read_request(uint32_t start_LBA, uint32_t no_LBA)
{
    // Validity Check
    if (start_LBA >= TOTAL_LBAs || start_LBA + no_LBA > TOTAL_LBAs)
    {
        printf("FE: Invalid LBA Range (Start: %u, Count: %u)\n", start_LBA, no_LBA);
        return;
    }

    printf("FE: Read request received: Start_LBA = %u, Count = %u\n", start_LBA, no_LBA);

    uint32_t end_LBA = start_LBA + no_LBA;
    uint32_t current_LBA = start_LBA;
    uint32_t no_of_LCA = fe_lba_lca(end_LBA - current_LBA);

    while (current_LBA < end_LBA)
    {
        uint32_t lca = current_LBA / 8;

        printf("FE: Converted LBA %u to LCA %u\n", current_LBA, lca);

        // Align LBA to next 16KB boundary (32 sectors)
        current_LBA += 32;

        for (uint32_t i = 0; i < 4; i++)
        {
            // Send to LUT for each LCA involved in a 16KB block
            if (!lut_read_request(lca++))
            {
                printf("0 ");
                // printf("FE: LUT read failed for LCA = %u\n", lca);
            }
            else
            {
                printf("1 ");
                // printf("FE: LUT read success for LCA = %u\n", lca);
            }
        }
    }

    printf("FE: Read request completed successfully.\n");
}

uint32_t fe_lba_lca(uint32_t lba)
{
    return lba / 8;
}

int main()
{
    printf("Total Device Capacity: %d MB\n", TOTAL_Device_Capacity);
    printf("Total Superblocks: %d\n", TOTAL_SUPERBLOCKS);
    printf("Total Physical Blocks: %d\n", TOTAL_PHYSICALBLCKS);
    printf("Total Physical Blocks per Super Block: %d\n", TOTAL_PHYSICAL_BLOCKS_PER_SUPER_BLOCK);
    printf("Total LCAs: %d\n", TOTAL_LCAs);
    printf("Total LBAs: %d\n", TOTAL_LBAs);
    printf("Page Size: %d KB\n", PAGE_SIZE);
    printf("Physical Block Size: %d MB\n", PHYSICALBLOCK_SIZE);
    printf("Super Block Size: %d MB\n", SUPERBLOCK_SIZE);

    return 0;
}
