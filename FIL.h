#ifndef FIL_H
#define FIL_H

#include <stdint.h>
#include "WC.h"
#include "LBM.h"
#include "MCA.h"

void fil_erase_block(LogicalBlock_t block);
uint32_t fil_write_request(uint32_t lca, MCA_t *mca);
uint32_t fil_read_request(MCA_t mca);

#endif