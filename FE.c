#include "FE.h"
#include "WC.h"
#include "LUT.h"
#include "MCA.h"
#include "LBM.h"
#include "FIL.h"

#include <stdio.h>
#include <stdint.h>

void fe_boot_modules()
{
    L2Pinit();
    LBMinit();
    MCA_t dummy_mca = {0};
    WCinit(&dummy_mca);
    LUTinit();
    // FILinit();
}

uint32_t fe_lba_lca(uint32_t lba)
{
    return lba / 8;
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

    while (current_LBA < end_LBA)
    {
        uint32_t lca = current_LBA / 8;
        current_LBA += 32; // Move to next 16KB aligned section

        for (uint32_t i = 0; i < 4; i++)
        {
            if (!wc_write_request(lca++))
                printf("0 ");
            else
                printf("1 ");
            // printf("Writing to LCA %u\n", lca++); // Simulate write operation
        }
    }

    printf("FE: Write request completed successfully.\n");
}

void fe_read_request(uint32_t start_LBA, uint32_t no_LBA)
{
    if (start_LBA >= TOTAL_LBAs || start_LBA + no_LBA > TOTAL_LBAs)
    {
        printf("FE: Invalid LBA Range (Start: %u, Count: %u)\n", start_LBA, no_LBA);
        return;
    }

    printf("FE: Read request received: Start_LBA = %u, Count = %u\n", start_LBA, no_LBA);

    uint32_t end_LBA = start_LBA + no_LBA;
    uint32_t current_LBA = start_LBA;

    while (current_LBA < end_LBA)
    {
        uint32_t lca = current_LBA / 8;
        current_LBA += 32;

        for (uint32_t i = 0; i < 4; i++)
        {
            if (!lut_read_request(lca++))
                printf("0 ");
            else
                printf("1 ");
        }
    }

    printf("FE: Read request completed successfully.\n");
}

int main()
{
    printf("Initializing FE modules...\n");
    fe_boot_modules();
    fe_write_request(0, 160);
    fe_read_request(0, 160);

    return 0;
}
