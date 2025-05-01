#ifndef LUT_H
#define LUT_H

#include <stdint.h>
#include "WC.h"  // For MCA_t
#include "MCA.h" // For MCA_t

#define UNMAPPED_VALUE -1 // Value to indicate unmapped entries

extern MCA_t LUT_Table[];

// Initializes the LUT with unmapped entries
void LUTinit(void);

// Updates the LUT with LCA → MCA mapping
void lut_update_request(uint32_t LCA, MCA_t *mca);

// Looks up the MCA for the given LCA and forwards the read request
uint32_t lut_read_request(uint32_t LCA);

#endif // LUT_H
