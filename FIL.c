#include <stdio.h>
#include <stdint.h>
#include "FIL.h"

void fil_erase_block(LogicalBlock_t block)
{
    // Simulate block erase operation
    printf("FIL: Erasing block with MBA %u\n", block.mba);
    // Here you would add the actual erase logic
}

uint32_t fil_write_request(uint32_t lca, MCA_t *mca)
{
    printf("FIL: Writing to block with MBA %u\n", mca->MBA);
    return 1; // Return success
}

uint32_t fil_read_request(MCA_t mca)
{
    printf("FIL: Reading from block with MBA %u\n", mca.MBA);
    return 1; // Return success
}